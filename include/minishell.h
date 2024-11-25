/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:32:36 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/11/25 00:24:35 by tcosta-f         ###   ########.fr       */
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
# include "errors.h"

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
	TOKEN_HEREDOC,
	TOKEN_BUILTIN,
	TOKEN_NULL,
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

typedef struct s_env
{
	char **envp;
	char *env_paths;
	char **paths;
	char *full_path;
}	t_env;

typedef struct s_minishell
{
	char				*input;
	t_token 			*tokens;
	t_node				*ast_root;
	t_env				env;
	int					status;
	int					save_stdin;
	int					save_stdout;
	int 				n_args;
	int					n_tokens;
	int					pid;
	struct	sigaction	sa;
	int					pipefd[2];
}				t_minishell;

/**__HANDLE_and_LEXING_INPUT__**/
int	ft_handle_and_tokenize_input(t_minishell *ms);
/**__HANDLE_INPUT__**/
int		ft_count_args(char *str);
int		ft_check_quotes(char *str);
/* void	ft_get_additional_input(char **str); */
int		ft_handle_quotes(char *str, int i, int *start, int *end);
int		ft_handle_argument(char *str, int i);

/**__LEXING_INPUT__**/
t_token	*ft_tokenize_input(char *str, int n_args, int i, int j);
int		ft_tokenize(char *str, int *i, t_token *tokens, int *j);
t_type	ft_get_token_type(char *str, t_type prev_type);
char	*ft_revalue_quoted_value(char *value);
int		ft_verify_variable_value(char *str);


/**__PARSING_AST_(Abstract Syntax Tree)__**/
t_node	*ft_parse_ast(t_token *tokens); // muito grande!
t_node	*ft_create_cmd_node(t_token *token);
t_node	*ft_create_operator_node(t_token *token, t_node *left, t_node *right);
t_node	*ft_group_command_tokens(t_token *tokens, int *index);
int		ft_verify_cmd_node_value(t_node *cmd_node);
char	*ft_remove_quotes(char *value);
int ft_value_has_space(char *value);
/* Temporaria para testar */
void	print_ast(t_node *node, int depth);

/**__EXECUTE_AST__ **/
int	ft_execute_ast(t_node *node, t_minishell *ms);
int	ft_handle_output_redirect(t_node *node, t_minishell *ms);
int	ft_handle_input_redirect(t_node *node, t_minishell *ms);
int	ft_handle_pipe(t_node *node, t_minishell *ms);
int	ft_execute_command(t_node *node, t_minishell *ms);
int	ft_handle_heredoc(t_node *node, t_minishell *ms);
//int	ft_handle_builtins(t_node *node, t_minishell *ms); //nao existe pois nao?
int	ft_find_executable(t_minishell *ms, char *cmd);

/**__BUILTINS__**/
int		ft_check_builtins(char *str);
void 	ft_builtin_exit(char **args);
void 	ft_builtin_pwd(void);
void 	ft_builtin_echo(char **args);
void	ft_builtin_env(char **args);

/**__SIGNAL__**/
void 	ft_signal_handler(int signum);

/**__FREE_MALLOCs_**/
void	ft_free_tokens(t_token *tokens);
void	ft_free_ast(t_node *root);
/* void	ft_free_env(t_env *env); */
void	ft_free_split(char **str);

/**__HANDLE_ENV__ **/
int		ft_revalue_token_variable(t_minishell *ms);
char	*ft_get_env(const char *key, char **envp);
char	**ft_duplicate_envp(char **envp);
int		ft_check_if_expand(char *str, char *ptr);
int		ft_replace_str(char **value, char *key, char *ptr, char *env_value);
char	*ft_get_env_value(const char *str, char **envp, char **key);


#endif
