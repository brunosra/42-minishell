/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:32:36 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/12/26 11:22:30 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	 MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "errors.h"
# include "colors.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <limits.h>
# include <stdbool.h>

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
	TOKEN_EXCEPT,
}	t_type;

typedef struct s_token
{
	t_type	type;
	char	*value;
	char	*old_value;

}	t_token;

/**__Parsing__**/
typedef struct s_ast_node
{
	t_token *token;
	char	**cmd_ready;
	t_node *left;
	t_node *right;
	t_node	*prev;
	bool	file;
	char	**heredoc_stops;
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
	pid_t				pid;
	int					pipefd[2];
	int					exit_code;
	bool				swap_input_redirects;
	bool				swap_output_redirects;
	int					c_multi_heredocs;
	bool					in_pipe;
	char				*temp;
	int					c_stuck_cats;
	char				*prompt;
}				t_minishell;

/**__HANDLE_and_LEXING_INPUT__**/
void ft_init_ms(t_minishell *ms);
int ft_save_stdin_stdout(t_minishell *ms);
int	ft_readline(t_minishell *ms);
void ft_close_stdin_stdout(t_minishell *ms);
int	ft_process_input_and_execute(t_minishell *ms);
int	ft_handle_and_tokenize_input(t_minishell *ms);
void ft_clean_stuck_cats(t_minishell *ms);
void ft_find_stuck_cats(t_minishell *ms, t_node *node);

/**__HANDLE_INPUT__**/
int		ft_count_args(char *str);
int		ft_skip_operator(const char *str, int i);
int		ft_check_quotes(char *str);
int		ft_handle_quotes(char *str, int i, int *start, int *end);
// static int	ft_skip_and_process(char *str, int i, char quote_type);
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
char 	**ft_remove_null_values(char **cmd_ready, int arg_count);
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
int	ft_invalid_right_token_value(char *value);
int	ft_is_valid_file(char *filepath, int mode);
void	ft_remove_created_files(t_node *node);
void	ft_create_files(t_node *node);
void	ft_swap_redirects_values(t_node *node, t_type type);
// int	ft_execute_heredocs(t_node *node, t_minishell *ms);
int	ft_collect_heredocs(t_node *node, t_minishell *ms);
int	ft_handle_multiple_heredocs(t_node *node, t_minishell *ms);

/**__BUILTINS__**/
int		ft_check_builtins(char *str);
void	ft_builtin_exit(char **args, t_minishell *ms);
void	ft_builtin_pwd(t_minishell *ms);
void	ft_builtin_echo(char **args, t_minishell *ms);
int		ft_value_is_numeric(char *str);
long long ft_atoll(char *str, int i, long long res);
void 	ft_builtin_env(char **args, t_minishell *ms); // deve retornar int
/* int		ft_builtin_cd(t_minishell *ms);
 */
/**__SIGNAL__**/
void	ft_signal_handler(int sig);
t_minishell	*ft_ms_struct(t_minishell *ms, int flag);
void	ft_set_main_signals(void);
void	ft_set_fork_signals(void);
void	ft_set_heredoc_signals(void);
void	ft_signal_heredoc_handler(int sig);

/**__FREE_MALLOCs_**/
void	ft_free_tokens(t_token *tokens);
void	ft_free_ast(t_node *root);
/* void	ft_free_env(t_env *env); */
void	ft_free_split(char **str);
char	*ft_strjoin_free(char *s1, char *s2, int free_s1, int free_s2);

/**__HANDLE_ENV__ **/
int		ft_revalue_token_variable(t_minishell *ms);
int		ft_check_balanced_quotes(char *str, int idx);
char	*ft_get_env(const char *key, char **envp);
char	**ft_duplicate_envp(char **envp);
int		ft_check_if_expand(char *str, char *ptr, int heredoc);
int		ft_replace_str(char **value, char *key, char *ptr, char *env_value);
char	*ft_get_env_value(const char *str, char **envp, char **key);
int		ft_revalue_heredock_input(char **input, t_minishell *ms);
/* int		ft_remove_str(char **value, char *key, char *ptr);
 */

/**__UTILS__ **/
int 	ft_perror(char *error, int return_value);
int 	ft_putstr_and_return(char *msg, int return_value);
void	set_exit_code(t_minishell *ms, int exit_code);
int		exit_code(t_minishell *ms);

#endif
