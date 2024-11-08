/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:32:36 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/11/08 03:06:21 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <asm-generic/signal.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>

# define t_node s_ast_node;

/**__Lexing__**/
typedef enum e_token_type
{
	TOKEN_COMMAND,
	TOKEN_ARGUMENT,
	TOKEN_OPERATOR,
	TOKEN_FILENAME,
	TOKEN_VARIABLE,
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
	t_node *left;
	t_node *right;
}	t_node;

/**__HANDLE_and_LEXING_INPUT__**/
static t_token	*ft_handle_and_tokenize_input(char *input);
/**__HANDLE_INPUT__**/
int				ft_count_args(char *str);
static int		ft_check_quotes(char *str);
static void		ft_get_additional_input(char **str);
int				ft_handle_quotes(char *str, int i);
static int		ft_handle_argument(char *str, int i);
/**__LEXING_INPUT__**/
t_token			*ft_tokenize_input(char *str, int n_args, int i, int j);
static int		ft_tokenize(char *str, int *i, t_token *tokens, int *j);
static t_type	ft_get_token_type(char *str);

/**__PARSING_AST_(Abstract Syntax Tree)__**/
//t_node *ft_parse_ast(t_token *token);

/**__BUILTINS__**/
void 			fT_builtin_exit(char **args);
void 			ft_builtin_pwd(void);
void 			ft_builtin_echo(char **args);

/**__SIGNAL__**/
void 			ft_signal_handler(int signum);

#endif
