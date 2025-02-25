/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:32:36 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/02/25 09:29:40 by tcosta-f         ###   ########.fr       */
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
	TKN_CMD,
	TKN_ARG,
	TKN_PIPE,
	TKN_FILE,
	TKN_VAR,
	TKN_OUT_RD,
	TKN_IN_RD,
	TKN_HDOC,
	TKN_BLTIN,
	TKN_NULL,
	TKN_EXCPT,
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
	bool				in_pipe;
	char				*temp;
	int					c_stuck_cats;
	char				*prompt;
	char				currpath[PATH_MAX];
}				t_minishell;

/**
 * @brief Structure to hold AST-related pointers for processing.
 */
typedef struct s_ast_helper
{
	t_node	**current;
	t_node	**root;
	t_node	*op_node;
}	t_ast_helper;

/**
 * @brief Structure to hold grouop_cmd-related variables for processing.
 */
typedef struct s_cmd_helper
{
	int		arg_count;
	int		i;
	int		j;
	int		len_value;
	int		n_args_cmd_nd_values;
	int		c_except;
	int		empty;
	int		stop;
	char	**cmd_nd_value;
}	t_cmd_helper;


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
int		ft_builtin_error(char *msg, int err);
char	*ft_strtok(char *str, const char *delim);
int		ft_exec_builtins_check(t_node *node, t_minishell *ms);
int		ft_exec_builtins(t_node *node, t_minishell *ms);
int		ft_check_builtins(char *str);
int		ft_builtin_exit(char **args);
int		ft_builtin_pwd();
void	ft_builtin_echo(char **args, t_minishell *ms);
void 	ft_builtin_env(char **args, t_minishell *ms);
void	ft_resolve_rel_p(const char *base_p, const char *rel_p, char *res_p);
int		ft_builtin_cd_check(char **args, t_minishell *ms);
void	ft_builtin_cd(char **args, t_minishell *ms);
void	ft_builtin_unset(char **args, t_minishell *ms);
/**__EXPORT__**/
int		ft_dup_envp(char **envp, char ***dupenv, int count);
int		ft_builtin_export_check(char **args/* , t_minishell *ms */);
void	ft_builtin_export(char **args, t_minishell *ms);
void	ft_export_1_arg(t_minishell *ms);

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
int		ft_revalue_tkn_var(t_minishell *ms);
int		ft_check_balanced_quotes(char *str, int idx);
char	**ft_duplicate_envp(char **envp);
int		ft_check_if_expand(char *str, char *ptr, int heredoc);
int		ft_replace_str(char **value, char *key, char *ptr, char *env_value);
char	*ft_get_env_value(const char *str, t_minishell *ms, char **key, bool heredoc);
int		ft_revalue_heredoc_input(char **input, t_minishell *ms);
char	*ft_get_env(const char *key, t_minishell *ms);
int		ft_set_env(const char *key, const char *value, t_minishell *ms);
int		ft_unset_env(const char *key, t_minishell *ms);

/* int		ft_remove_str(char **value, char *key, char *ptr);
 */

/**__UTILS__ **/
int 	ft_perror(char *error, int return_value);
int 	ft_putstr_and_return(char *msg, int return_value);
void	ft_set_exit_code(t_minishell *ms, int exit_code);
int		ft_exit_code(t_minishell *ms);

/**__MISC_UTILS__**/
void	ft_print_str_arr(char **arr);
void	ft_swap(char **a, char **b);
void	ft_free_str_arr(char **arr);

#endif
