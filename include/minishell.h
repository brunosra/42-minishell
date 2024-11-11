/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:32:36 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/11/11 19:10:40 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	 MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>

typedef struct s_ast_node t_node;

/**__Lexing__**/
typedef enum e_token_type
{
	TOKEN_COMMAND,
	TOKEN_ARGUMENT,
	TOKEN_OPERATOR,
	TOKEN_FILENAME,
	TOKEN_VARIABLE,
	TOKEN_OUTPUT_REDIRECT,
	TOKEN_INPUT_REDIRECT,
}	t_type;

typedef struct s_token
{
	t_type	type;
	char	*value;
}	t_token;

/**__Parsing__**/
typedef struct s_ast_node
{
	t_token *token;
	char	**cmd_ready;
	t_node *left;
	t_node *right;
}	t_node;

/**__HANDLE_and_LEXING_INPUT__**/
t_token	*ft_handle_and_tokenize_input(char *input);
/**__HANDLE_INPUT__**/
int				ft_count_args(char *str);
int		ft_check_quotes(char *str);
void		ft_get_additional_input(char **str);
int				ft_handle_quotes(char *str, int i);
int		ft_handle_argument(char *str, int i);
/**__LEXING_INPUT__**/
t_token			*ft_tokenize_input(char *str, int n_args, int i, int j);
int		ft_tokenize(char *str, int *i, t_token *tokens, int *j);
t_type	ft_get_token_type(char *str, t_type prev_type);

/**__PARSING_AST_(Abstract Syntax Tree)__**/
t_node	*ft_parse_ast(t_token *tokens); // muito grande!
t_node	*ft_create_cmd_node(t_token *token);
t_node	*ft_create_operator_node(t_token *token, t_node *left, t_node *right);
t_node	*ft_group_command_tokens(t_token *tokens, int *index);
/* Temporaria para testar */
void	print_ast(t_node *node, int depth);

/**__EXECUTE_AST__ **/
int	ft_execute_ast(t_node *node);
int	ft_handle_output_redirect(t_node *node);
int	ft_handle_input_redirect(t_node *node);
int	ft_handle_pipe(t_node *node);
int	ft_execute_command(t_node *node);

/**__BUILTINS__**/
void 			fT_builtin_exit(char **args);
void 			ft_builtin_pwd(void);
void 			ft_builtin_echo(char **args);

/**__SIGNAL__**/
void 			ft_signal_handler(int signum);

#endif
