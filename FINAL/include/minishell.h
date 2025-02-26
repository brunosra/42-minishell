/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:32:36 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/02/26 08:04:56 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
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

/* ************************************************************************** */
/*                              STRUCTURES                                    */
/* ************************************************************************** */

typedef struct s_ast_node	t_node;

/* __ Lexing __ */
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

/* __ Parsing __ */
typedef struct s_ast_node
{
	t_token		*token;
	char		**cmd_ready;
	t_node		*left;
	t_node		*right;
	t_node		*prev;
	bool		file;
	char		**heredoc_stops;
}	t_node;

/* __ Handle_ENV __ */
typedef struct s_env
{
	char	**envp;
	char	*env_paths;
	char	**paths;
	char	*full_path;
}	t_env;

typedef struct s_minishell
{
	char		*input;
	t_token		*tokens;
	t_node		*ast_root;
	t_env		env;
	int			status;
	int			save_stdin;
	int			save_stdout;
	int			n_args;
	pid_t		pid;
	int			pipefd[2];
	int			exit_code;
	bool		swap_input_redirects;
	bool		swap_output_redirects;
	int			c_multi_heredocs;
	bool		in_pipe;
	char		*temp;
	int			c_stuck_cats;
	char		*prompt;
	char		currpath[PATH_MAX];
}	t_minishell;

/* __ AST Helper Structures __ */
typedef struct s_ast_helper
{
	t_node		**current;
	t_node		**root;
	t_node		*op_node;
}	t_ast_helper;

typedef struct s_cmd_helper
{
	int			arg_count;
	int			i;
	int			j;
	int			len_value;
	int			n_args_cmd_nd_values;
	int			c_except;
	int			empty;
	int			stop;
	char		**cmd_nd_value;
}	t_cmd_helper;

/* ************************************************************************** */
/*                              FUNCTION HEADERS                              */
/* ************************************************************************** */

/* __ Minishell Main __ */
int				main(int argc, char **argv, char **envp);
void			ft_create_prompt(t_minishell *ms);

/* __ Process Input __ */
int				ft_process_input_and_execute(t_minishell *ms);

/* __ Handle Input __ */
int				ft_check_quotes(char *str);
int				ft_count_args(char *str);
int				ft_handle_quotes(char *str, int i, int *start, int *end);

/* __ Lexing Input __ */
t_token			*ft_tokenize_input(char *str, int n_args, int i, int j);
t_type			ft_get_token_type(char *str, t_type prev_type);
int				ft_check_builtins(char *str);
int				ft_verify_variable_value(char *str, int i);
char			*ft_revalue_quoted_value(char *value);

/* __ Parsing AST __ */
t_node			*ft_parse_ast(t_token *tokens);
t_node			*ft_create_cmd_node(t_token *token);
t_node			*ft_create_operator_node(t_token *token, t_node *left,
					t_node *right);
t_node			*ft_group_command_tokens(t_token *tokens, int *index);
void			ft_collect_arguments(t_token *tokens, int *index,
					t_cmd_helper *h);
t_cmd_helper	ft_init_cmd_helper(t_node *cmd_node);
int				ft_verify_cmd_node_value(t_node *cmd_node);
char			**ft_remove_null_values(char **cmd_ready, int arg_count);

/* __ Execution __ */
int				ft_execute_ast(t_node *node, t_minishell *ms);
int				ft_handle_heredoc(t_node *node, t_minishell *ms);
int				ft_handle_multiple_heredocs(t_node *node, t_minishell *ms);
int				ft_handle_output_redirect(t_node *node, t_minishell *ms);
int				ft_handle_input_redirect(t_node *node, t_minishell *ms);
int				ft_handle_pipe(t_node *node, t_minishell *ms);
int				ft_execute_command(t_node *node, t_minishell *ms);
int				ft_find_executable(t_minishell *ms, char *cmd);
int				ft_is_valid_file(char *filepath, int mode);
void			ft_remove_created_files(t_node *node);
void			ft_create_files(t_node *node);

/* __ Builtins __ */
int				ft_builtin_error(char *msg, int err);
int				ft_exec_builtins_check(t_node *node, t_minishell *ms);
int				ft_exec_builtins(t_node *node, t_minishell *ms);
void			ft_builtin_cd(char **args, t_minishell *ms);
int				ft_builtin_cd_check(char **args, t_minishell *ms);
void			ft_builtin_echo(char **args, t_minishell *ms);
void			ft_builtin_env(char **args, t_minishell *ms);
int				ft_builtin_exit(char **args);
int				ft_builtin_export_check(char **args);
void			ft_builtin_export(char **args, t_minishell *ms);
int				ft_builtin_pwd(void);
void			ft_builtin_unset(char **args, t_minishell *ms);

/* __ Environment Handling __ */
int				ft_revalue_tkn_var(t_minishell *ms);
int				ft_check_if_expand(char *str, char *ptr, int heredoc);
int				ft_replace_str(char **value, char *key, char *ptr,
					char *env_value);
int				ft_revalue_heredoc_input(char **input, t_minishell *ms);
char			*ft_get_env_value(const char *str, t_minishell *ms, char **key,
					bool heredoc);
char			*ft_get_env(const char *key, t_minishell *ms);
int				ft_unset_env(const char *key, t_minishell *ms);
int				ft_set_env(const char *key, const char *value, t_minishell *ms);

/* __ Memory Management __ */
void			ft_free_tokens(t_token *tokens);
void			ft_free_ast(t_node *root);
void			ft_free_split(char **str);

/* __ Signal Handling __ */
void			ft_signal_handler(int sig);
void			ft_set_main_signals(void);
void			ft_set_fork_signals(void);
void			ft_set_heredoc_signals(void);
void			ft_signal_heredoc_handler(int sig);

/* __ Utility Functions __ */
int				ft_perror(char *error, int return_value);
int				ft_putstr_and_return(char *msg, int return_value);
void			ft_set_exit_code(t_minishell *ms, int exit_code);
int				ft_exit_code(t_minishell *ms);
char			*ft_str_join_all(int argcount, ...);
char			*ft_strjoin_free(char *s1, char *s2, int free_s1, int free_s2);
t_minishell		*ft_ms_struct(t_minishell *ms, int flag);
void			ft_print_ast(t_node *node, int depth);
#endif
