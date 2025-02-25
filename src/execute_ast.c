/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:54:54 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/02/25 02:33:09 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
extern volatile int	g_interrupt;

int	ft_execute_ast(t_node *node, t_minishell *ms);
static int	ft_handle_syntax_error(t_node *node, t_minishell *ms);
static int	ft_execute_output_redirect(t_node *node, t_minishell *ms);
static int	ft_execute_input_redirect(t_node *node, t_minishell *ms);
static int	ft_execute_heredoc(t_node *node, t_minishell *ms);
int	ft_handle_heredoc(t_node *node, t_minishell *ms);
static int	ft_heredoc_syntax_error(t_minishell *ms);
static int	ft_handle_heredoc_fork_error(t_minishell *ms);
static void	ft_heredoc_child_process(t_node *node, t_minishell *ms);
static char	*ft_update_heredoc_buffer(char *temp, char *input);
static void	ft_write_heredoc(t_minishell *ms, t_node *node, char *temp);
static void	ft_restore_stdin(t_minishell *ms);
static int	ft_handle_exit_status(t_minishell *ms, t_node *node);
static int	ft_redirect_stdout(t_minishell *ms);
static void	ft_dup2_error(t_minishell *ms, int tty_fd, int save_stdout);
static int	ft_open_tty(t_minishell *ms, int save_stdout);
static int	ft_duplicate_stdout(t_minishell *ms);
void	ft_restore_stdout(int save_stdout, t_minishell *ms);
int	ft_handle_output_redirect(t_node *node, t_minishell *ms);
static int	ft_check_redirect_syntax(t_node *node, t_minishell *ms);
static int	ft_open_output_file(t_node *node);
static int	ft_handle_file_error(t_minishell *ms);
static int	ft_handle_dup_error(int fd, t_minishell *ms);
int	ft_handle_input_redirect(t_node *node, t_minishell *ms);
static int	ft_validate_input_file(t_node *node, t_minishell *ms);
int	ft_handle_pipe(t_node *node, t_minishell *ms);
static int	ft_redirect_pipe_input(t_minishell *ms);
static void	ft_execute_pipe_child(t_node *node, t_minishell *ms);
static int	ft_handle_fork_error(t_minishell *ms);
int	ft_create_pipe(t_minishell *ms);
static int	ft_check_pipe_syntax(t_node *node, t_minishell *ms);
static int	ft_pipe_syntax_error(t_minishell *ms, char *token, int code);
static void	ft_handle_unfinished_pipe(t_minishell *ms, char *input);
int	ft_execute_command(t_node *node, t_minishell *ms);
static void	ft_handle_cmd_exit_status(t_node *node, t_minishell *ms);
static void	ft_execute_child_process(t_node *node, t_minishell *ms);
static int	ft_execute_external(t_node *node, t_minishell *ms);
static void	ft_execute_builtin(t_node *node, t_minishell *ms);
int	ft_find_executable(t_minishell *ms, char *cmd);
int	ft_invalid_right_token_value(char *value);
int	ft_is_valid_file(char *filepath, int mode);
static int	ft_check_file_access(char *filepath, int mode);
static int	ft_file_error(char *filepath, char *msg, int code);
void	ft_swap_redirects_values(t_node *node, t_type type);
void	ft_remove_created_files(t_node *node);
void	ft_create_files(t_node *node);
static int ft_has_cat(t_node *node);
int	ft_handle_multiple_heredocs(t_node *node, t_minishell *ms);
static int	ft_fork_heredoc(t_minishell *ms, t_node *node, int *i);
static void	ft_cleanup_heredocs(t_minishell *ms, int save_stdout, t_node *node);
static void	ft_multiple_heredoc_child(t_node *node, t_minishell *ms, int *i);
static int	ft_process_heredoc_input(t_node *node, t_minishell *ms,
										char *input, int *i);
static void	ft_finalize_heredoc(t_minishell *ms, int *i);
int	ft_collect_heredocs(t_node *node, t_minishell *ms);
static int	ft_allocate_heredoc_stops(t_node *node, t_minishell *ms);
static int	ft_multiple_heredoc_syntax_error(t_node *current, t_minishell *ms);

// /**
//  * @brief  Executes the AST based on the current node type.
//  * 
//  * @param  node  Pointer to the current AST node.
//  * @param  ms    Pointer to the minishell structure.
//  * @return int   Execution status.
//  **         0 on success.
//  **         Non-zero in case of errors or syntax issues.
//  */
// int	ft_execute_ast(t_node *node, t_minishell *ms)
// {
// 	if (!node || !ms->n_args)
// 		return (1);
//  	if (node->token->type == TKN_EXCPT)
// 	{
// 		ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
// 		ft_putstr_fd(node->token->value, STDERR_FILENO);
// 		ft_putstr_fd("'\n", STDERR_FILENO);
// 		ft_set_exit_code(ms, 2);
		
// 		return (1);
// 	}
// 	if (node->token->type == TKN_OUT_RD)
// 	{
// 		if (ms->swap_output_redirects == false)
// 		{
// 			ft_swap_redirects_values(node, TKN_OUT_RD);
// 			ms->swap_output_redirects = true;
// 		}
// 		return (ft_handle_output_redirect(node, ms));
// 	}
// 	else if (node->token->type == TKN_IN_RD)
// 	{
// 		if (ms->swap_input_redirects == false)
// 		{
// 			ft_swap_redirects_values(node, TKN_IN_RD);
// 			ms->swap_input_redirects = true;
// 		}
// 		return (ft_handle_input_redirect(node, ms));
// 	}
//  	else if (node->token->type == TKN_HDOC)
// 	{
// 		if (node->left && node->left->token->type == TKN_HDOC)
// 		{
// 			if (ft_collect_heredocs(node, ms))
// 				return (1);
// 			return (ft_handle_multiple_heredocs(node, ms));
// 		}
// 		return (ft_handle_heredoc(node, ms));
// 	}
//  	else if (node->token->type == TKN_PIPE)
// 		return (ft_handle_pipe(node, ms));
// 	else if (node->token->type == TKN_BLTIN)
// 		return (ft_execute_command(node, ms));
// 	else if (node->token->type == TKN_CMD)
// 		return (ft_execute_command(node, ms));
// 	return (0);
// }

/**
 * @brief  Executes the AST based on the current node type.
 * 
 * @param  node  Pointer to the current AST node.
 * @param  ms    Pointer to the minishell structure.
 * @return int   Execution status.
 *         0 on success.
 *         Non-zero in case of errors or syntax issues.
 */
int	ft_execute_ast(t_node *node, t_minishell *ms)
{
	if (!node || !ms->n_args)
		return (1);
	if (node->token->type == TKN_EXCPT)
		return (ft_handle_syntax_error(node, ms));
	if (node->token->type == TKN_OUT_RD)
		return (ft_execute_output_redirect(node, ms));
	if (node->token->type == TKN_IN_RD)
		return (ft_execute_input_redirect(node, ms));
	if (node->token->type == TKN_HDOC)
		return (ft_execute_heredoc(node, ms));
	if (node->token->type == TKN_PIPE)
		return (ft_handle_pipe(node, ms));
	if (node->token->type == TKN_BLTIN ||
		node->token->type == TKN_CMD)
		return (ft_execute_command(node, ms));
	return (0);
}

/**
 * @brief  Handles syntax errors related to unexpected tokens.
 * 
 * @param  node  Pointer to the AST node with the error.
 * @param  ms    Pointer to the minishell structure.
 * @return int   Always returns 1 to indicate an error.
 */
static int	ft_handle_syntax_error(t_node *node, t_minishell *ms)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	ft_putstr_fd(node->token->value, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	ft_set_exit_code(ms, 2);
	return (1);
}

/**
 * @brief  Handles output redirection in the AST execution.
 * 
 * @param  node  Pointer to the output redirection node.
 * @param  ms    Pointer to the minishell structure.
 * @return int   Execution status.
 */
static int	ft_execute_output_redirect(t_node *node, t_minishell *ms)
{
	if (ms->swap_output_redirects == false)
	{
		ft_swap_redirects_values(node, TKN_OUT_RD);
		ms->swap_output_redirects = true;
	}
	return (ft_handle_output_redirect(node, ms));
}

/**
 * @brief  Handles input redirection in the AST execution.
 * 
 * @param  node  Pointer to the input redirection node.
 * @param  ms    Pointer to the minishell structure.
 * @return int   Execution status.
 */
static int	ft_execute_input_redirect(t_node *node, t_minishell *ms)
{
	if (ms->swap_input_redirects == false)
	{
		ft_swap_redirects_values(node, TKN_IN_RD);
		ms->swap_input_redirects = true;
	}
	return (ft_handle_input_redirect(node, ms));
}

/**
 * @brief  Handles heredoc execution in the AST.
 * 
 * @param  node  Pointer to the heredoc node.
 * @param  ms    Pointer to the minishell structure.
 * @return int   Execution status.
 */
static int	ft_execute_heredoc(t_node *node, t_minishell *ms)
{
	if (node->left && node->left->token->type == TKN_HDOC)
	{
		if (ft_collect_heredocs(node, ms))
			return (1);
		return (ft_handle_multiple_heredocs(node, ms));
	}
	return (ft_handle_heredoc(node, ms));
}


/**
 * @brief  Swaps the values of redirect nodes to handle nested or reversed cases.
 * 
 * @param  node  Pointer to the current node in the AST.
 * @param  type  Redirect type (input/output).
 */
void	ft_swap_redirects_values(t_node *node, t_type type)
{
	t_node	*current;
	char	*temp_value;

	if (!node || node->token->type != type)
		return;
	current = node;
	while (current && current->left && current->left->token->type == type)
		current = current->left;
	if (current != node) // Troca os valores do último nó (mais profundo) com o atual
	{
		temp_value = current->right->token->value;
		current->right->token->value = node->right->token->value;
		node->right->token->value = temp_value;
	}
}

// /**
//  * @brief  Handles heredoc redirection by reading user input until a delimiter is reached.
//  * 
//  * @param  node  Pointer to the heredoc node in the AST.
//  * @param  ms    Pointer to the minishell structure.
//  * @return int   Execution status.
//  **         0 on success.
//  **         Non-zero in case of errors or syntax issues.
//  */
// int	ft_handle_heredoc(t_node *node, t_minishell *ms)
// {
// 	char	*input;
// 	char	*temp;
// 	int		save_stdout;
// 	int		tty_fd;

// 	input = NULL;
// 	temp = NULL;
// 	save_stdout = -1;
// 	tty_fd = -1;
// 	if (!node->right)
// 	{
// 		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
// 		ft_set_exit_code(ms, 2);
// 		return (1);
// 	}
// 	if (pipe(ms->pipefd) == -1)
// 	{
// 		perror("pipe");
// 		ft_set_exit_code(ms, 1);
// 		return (1);
// 	}
// 	if (!isatty(STDOUT_FILENO))
// 	{
// 		save_stdout = dup(STDOUT_FILENO);
// 		if (save_stdout == -1)
// 		{
// 			perror("dup");
// 			close(ms->pipefd[0]);
// 			close(ms->pipefd[1]);
// 			ft_set_exit_code(ms, 1);
// 			return (1);
// 		}
// 		tty_fd = open("/dev/tty", O_WRONLY);
// 		if (tty_fd == -1)
// 		{
// 			perror("open /dev/tty");
// 			close(ms->pipefd[0]);
// 			close(ms->pipefd[1]);
// 			close(save_stdout);
// 			ft_set_exit_code(ms, 1);
// 			return (1);
// 		}
// 		if (dup2(ms->save_stdout, STDOUT_FILENO) == -1)
// 		{
// 			perror("dup2");
// 			close(tty_fd);
// 			close(ms->pipefd[0]);
// 			close(ms->pipefd[1]);
// 			close(ms->save_stdout);
// 			close(save_stdout);
// 			ft_set_exit_code(ms, 1);
// 			return (1);
// 		}
// 		close(tty_fd);
// 	}
// 	ms->pid = fork();
// 	if (ms->pid == -1)
// 	{
// 		perror("fork");
// 		close(ms->pipefd[0]);
// 		close(ms->pipefd[1]);
// 		ft_set_exit_code(ms, 1);
// 		return (1);
// 	}
// 	ft_set_fork_signals();
// 	if (ms->pid == 0)
// 	{
// 		ft_set_heredoc_signals();
// 		close(ms->pipefd[0]);
// 		while (1)
// 		{
// 			input = readline("> ");
//  			if (input == NULL) // Ctrl-D ou EOF
// 			{
// 				ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", STDERR_FILENO);
// 				ft_putstr_fd(node->right->token->value, STDERR_FILENO);
// 				write(STDERR_FILENO, "')\n", 3); 
// 				ft_set_exit_code(ms, 0);
// 				break; // Contar heredocs, criar variavel para saber qdo tem de fechar exit 130
// 			} 
// 			if (!input || ft_strcmp(input, node->right->token->value) == 0)
// 				break;
// 			if (temp)
// 			{
// 				temp = ft_strjoin_free(temp, input, 1, 0);
// 				temp = ft_strjoin_free(temp, "\n", 1, 0);
// 			}
// 			else
// 			{
// 				temp = ft_strdup(input);
// 				temp = ft_strjoin_free(temp, "\n", 1, 0);
// 			}
// 			free(input);
// 		}
// 		if (temp)
// 		{
// 			if (node->right && node->right->token->old_value[0] != '"' && node->right->token->old_value[0] != '\'')
// 				ft_revalue_heredock_input(&temp, ms);
// 			write(ms->pipefd[1], temp, ft_strlen(temp));
// 		}
// 		close(ms->pipefd[1]);
// 		exit(0);
// 	}
// 	close(ms->pipefd[1]);
// 	if (dup2(ms->pipefd[0], STDIN_FILENO) == -1)
// 	{
// 		perror("dup2");
// 		close(ms->pipefd[0]);
// 		ft_set_exit_code(ms, 1);
// 		return (1);
// 	}
// 	close(ms->pipefd[0]);
// 	waitpid(ms->pid, &ms->status, 0);
// 	ft_set_main_signals();
// 	if (save_stdout != -1)
// 	{
// 		if (dup2(save_stdout, STDOUT_FILENO) == -1)
// 		{
// 			perror("dup2");
// 			close(save_stdout);
// 			ft_set_exit_code(ms, 1);
// 			return (1);
// 		}
// 		close(save_stdout);
// 	}
// 	if (WIFEXITED(ms->status))
// 		ft_set_exit_code(ms, WEXITSTATUS(ms->status));
// 	else
// 		ft_set_exit_code(ms, 1);
// 	return (ft_execute_ast(node->left, ms));
// }

/**
 * @brief  Handles heredoc redirection by reading user input until a delimiter is reached.
 * 
 * @param  node  Pointer to the heredoc node in the AST.
 * @param  ms    Pointer to the minishell structure.
 * @return int   Execution status.
 **         0 on success.
 **         Non-zero in case of errors or syntax issues.
 */
int	ft_handle_heredoc(t_node *node, t_minishell *ms)
{
	int	save_stdout;
	int	pipe_status;

	save_stdout = -1;
	if (!node->right)
		return (ft_heredoc_syntax_error(ms));
	pipe_status = ft_create_pipe(ms);
	if (pipe_status)
		return (pipe_status);
	if (!isatty(STDOUT_FILENO))
		save_stdout = ft_redirect_stdout(ms);
	ms->pid = fork();
	if (ms->pid == -1)
		return (ft_handle_heredoc_fork_error(ms));
	ft_set_fork_signals();
	if (ms->pid == 0)
		ft_heredoc_child_process(node, ms);
	close(ms->pipefd[1]);
	waitpid(ms->pid, &ms->status, 0);
	ft_set_main_signals();
	ft_restore_stdin(ms);
	if (save_stdout != -1)
		ft_restore_stdout(save_stdout, ms);
	return (ft_handle_exit_status(ms, node));
}

/**
 * @brief  Creates a pipe for heredoc input.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return int  0 on success, 1 on failure.
 */
int	ft_create_pipe(t_minishell *ms)
{
	if (pipe(ms->pipefd) == -1)
	{
		perror("pipe");
		ft_set_exit_code(ms, 1);
		return (1);
	}
	return (0);
}

/**
 * @brief  Handles syntax errors in heredoc usage.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return int  Always returns 1 (error).
 */
static int	ft_heredoc_syntax_error(t_minishell *ms)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n",
		STDERR_FILENO);
	ft_set_exit_code(ms, 2);
	return (1);
}

/**
 * @brief  Handles a fork error.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return int  Always returns 1 (error).
 */
static int	ft_handle_heredoc_fork_error(t_minishell *ms)
{
	perror("fork");
	close(ms->pipefd[0]);
	close(ms->pipefd[1]);
	ft_set_exit_code(ms, 1);
	return (1);
}

/**
 * @brief  Handles the child process for heredoc input.
 * 
 * @param  node  Pointer to the heredoc node.
 * @param  ms    Pointer to the minishell structure.
 */
static void	ft_heredoc_child_process(t_node *node, t_minishell *ms)
{
	char	*temp;
	char	*input;

	ft_set_heredoc_signals();
	close(ms->pipefd[0]);
	temp = NULL;
	while (1)
	{
		input = readline("> ");
		if (!input)
			break;
		if (!ft_strcmp(input, node->right->token->value))
			break;
		temp = ft_update_heredoc_buffer(temp, input);
		free(input);
	}
	ft_write_heredoc(ms, node, temp);
	close(ms->pipefd[1]);
	exit(0);
}

/**
 * @brief  Appends user input to the heredoc buffer.
 * 
 * @param  temp  Current buffer.
 * @param  input User input line.
 * @return char* Updated buffer.
 */
static char	*ft_update_heredoc_buffer(char *temp, char *input)
{
	char	*new_temp;

	if (temp)
	{
		new_temp = ft_strjoin_free(temp, input, 1, 0);
		new_temp = ft_strjoin_free(new_temp, "\n", 1, 0);
	}
	else
	{
		new_temp = ft_strdup(input);
		new_temp = ft_strjoin_free(new_temp, "\n", 1, 0);
	}
	return (new_temp);
}

/**
 * @brief  Writes the heredoc content to the pipe.
 * 
 * @param  ms    Pointer to the minishell structure.
 * @param  node  Pointer to the heredoc node.
 * @param  temp  Buffer containing heredoc content.
 */
static void	ft_write_heredoc(t_minishell *ms, t_node *node, char *temp)
{
	if (temp)
	{
		if (node->right && node->right->token->old_value[0] != '"'
			&& node->right->token->old_value[0] != '\'')
			ft_revalue_heredock_input(&temp, ms);
		write(ms->pipefd[1], temp, ft_strlen(temp));
		free(temp);
	}
}

/**
 * @brief  Restores stdin from the pipe.
 * 
 * @param  ms  Pointer to the minishell structure.
 */
static void	ft_restore_stdin(t_minishell *ms)
{
	if (dup2(ms->pipefd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(ms->pipefd[0]);
		ft_set_exit_code(ms, 1);
	}
	close(ms->pipefd[0]);
}

/**
 * @brief  Handles the exit status after the heredoc process.
 * 
 * @param  ms    Pointer to the minishell structure.
 * @param  node  Pointer to the heredoc node.
 * @return int   Execution status.
 */
static int	ft_handle_exit_status(t_minishell *ms, t_node *node)
{
	if (WIFEXITED(ms->status))
	{
		ft_set_exit_code(ms, WEXITSTATUS(ms->status));
		// ft_restore_stdin(ms);
	}
	if (WIFEXITED(ms->status) && WEXITSTATUS(ms->status) == 130)
    {
            ft_set_exit_code(ms, 130);
            // close(ms->pipefd[0]);
			return (1);
    }
	// else if (WIFSIGNALED(ms->status)) // Processo foi terminado por um sinal
	// {
	// 	if (ms->status == SIGINT)
	// 	{
	// 		ft_set_exit_code(ms, 130);
	// 		write(STDERR_FILENO, "\n", 1);
	// 		close(ms->pipefd[0]);
	// 	}
	// 	else
	// 		ft_set_exit_code(ms, WEXITSTATUS(ms->status));
	// }
	else
		ft_set_exit_code(ms, 1);
	return (ft_execute_ast(node->left, ms));
}

/**
 * @brief  Redirects STDOUT to /dev/tty when needed.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return int  File descriptor of saved stdout.
 */
static int	ft_redirect_stdout(t_minishell *ms)
{
	int	save_stdout;
	int	tty_fd;

	save_stdout = ft_duplicate_stdout(ms);
	if (save_stdout == -1)
		return (-1);
	tty_fd = ft_open_tty(ms, save_stdout);
	if (tty_fd == -1)
		return (-1);
	if (dup2(ms->save_stdout, STDOUT_FILENO) == -1)
	{
		ft_dup2_error(ms, tty_fd, save_stdout);
		return (-1);
	}
	close(tty_fd);
	return (save_stdout);
}

/**
 * @brief  Duplicates STDOUT file descriptor.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return int  Duplicated file descriptor or -1 on error.
 */
static int	ft_duplicate_stdout(t_minishell *ms)
{
	int	save_stdout;

	save_stdout = dup(STDOUT_FILENO);
	if (save_stdout == -1)
	{
		perror("dup");
		close(ms->pipefd[0]);
		close(ms->pipefd[1]);
		ft_set_exit_code(ms, 1);
	}
	return (save_stdout);
}

/**
 * @brief  Opens /dev/tty for writing.
 * 
 * @param  ms           Pointer to the minishell structure.
 * @param  save_stdout  Saved stdout file descriptor.
 * @return int          File descriptor of /dev/tty or -1 on error.
 */
static int	ft_open_tty(t_minishell *ms, int save_stdout)
{
	int	tty_fd;

	tty_fd = open("/dev/tty", O_WRONLY);
	if (tty_fd == -1)
	{
		perror("open /dev/tty");
		close(ms->pipefd[0]);
		close(ms->pipefd[1]);
		close(save_stdout);
		ft_set_exit_code(ms, 1);
	}
	return (tty_fd);
}

/**
 * @brief  Handles dup2 errors and cleans up resources.
 * 
 * @param  ms           Pointer to the minishell structure.
 * @param  tty_fd       File descriptor of /dev/tty.
 * @param  save_stdout  Saved stdout file descriptor.
 */
static void	ft_dup2_error(t_minishell *ms, int tty_fd, int save_stdout)
{
	perror("dup2");
	close(tty_fd);
	close(ms->pipefd[0]);
	close(ms->pipefd[1]);
	close(ms->save_stdout);
	close(save_stdout);
	ft_set_exit_code(ms, 1);
}


/**
 * @brief  Restores the original STDOUT.
 * 
 * @param  save_stdout  File descriptor of the saved stdout.
 */
void	ft_restore_stdout(int save_stdout, t_minishell *ms)
{
	if (dup2(save_stdout, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(save_stdout);
		ft_set_exit_code(ms, 1);
	}
	close(save_stdout);
}


// /**
//  * @brief  Handles output redirection to a file.
//  * 
//  * @param  node  Pointer to the output redirection node in the AST.
//  * @param  ms    Pointer to the minishell structure.
//  * @return int   Execution status.
//  **         0 on success.
//  **         Non-zero in case of errors or syntax issues.
//  */
// int	ft_handle_output_redirect(t_node *node, t_minishell *ms)
// {
// 	int	fd;

// 	if (!node->right) // Verifica se o token à direita é inválido
// 	{
// 		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
// 		ft_set_exit_code(ms, 2);
// 		return (1);
// 	}
// 	else if (ft_invalid_right_token_value(node->right->token->value))
// 	{
// 		ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
// 		if (node->right->token->value)
// 			ft_putstr_fd(node->right->token->value, STDERR_FILENO);
// 		else
// 			ft_putstr_fd("newline", STDERR_FILENO);
// 		ft_putstr_fd("'\n", STDERR_FILENO);
// 		ft_set_exit_code(ms, 2);
// 		return (1);
// 	}
// /* 	if (ft_is_valid_file(node->right->token->value, O_WRONLY)) // Verifica se o arquivo é inválido// verificar se e um diretorio!!
// 	{
// 		ft_set_exit_code(ms, 1);
// 		return (1);
// 	} */
// 	if (ft_strcmp(node->token->value, ">>") == 0) // Abre o arquivo para escrita ou append
// 		fd = open(node->right->token->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 	else
// 		fd = open(node->right->token->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd == -1) // Trata erros ao abrir o arquivo
// 	{
// 		perror("open");
// 		ft_set_exit_code(ms, 1);
// 		return (1);
// 	}
// 	else
// 		node->file = true;
// 	if (dup2(fd, STDOUT_FILENO) == -1) // Redireciona STDOUT para o arquivo
// 	{
// 		perror("dup2");
// 		close(fd);
// 		ft_set_exit_code(ms, 1);
// 		return (1);
// 	}
// 	close(fd);
// 	if (node->left)
// 		return (ft_execute_ast(node->left, ms));
// 	else
// 	{
// 		ft_set_exit_code(ms, 0);
// 		return (0);
// 	}
// }

/**
 * @brief  Handles output redirection to a file.
 * 
 * @param  node  Pointer to the output redirection node in the AST.
 * @param  ms    Pointer to the minishell structure.
 * @return int   Execution status.
 *         0 on success.
 *         Non-zero in case of errors or syntax issues.
 */
int	ft_handle_output_redirect(t_node *node, t_minishell *ms)
{
	int	fd;

	if (ft_check_redirect_syntax(node, ms))
		return (1);
	fd = ft_open_output_file(node);
	if (fd == -1)
		return (ft_handle_file_error(ms));
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (ft_handle_dup_error(fd, ms));
	close(fd);
	if (node->left)
		return (ft_execute_ast(node->left, ms));
	ft_set_exit_code(ms, 0);
	return (0);
}

/**
 * @brief  Checks for syntax errors in output redirection.
 * 
 * @param  node  Pointer to the output redirection node.
 * @param  ms    Pointer to the minishell structure.
 * @return int   1 if there is a syntax error, 0 otherwise.
 */
static int	ft_check_redirect_syntax(t_node *node, t_minishell *ms)
{
	if (!node->right)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n",
			STDERR_FILENO);
		ft_set_exit_code(ms, 2);
		return (1);
	}
	if (ft_invalid_right_token_value(node->right->token->value))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `",
			STDERR_FILENO);
		if (node->right->token->value)
			ft_putstr_fd(node->right->token->value, STDERR_FILENO);
		else
			ft_putstr_fd("newline", STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
		ft_set_exit_code(ms, 2);
		return (1);
	}
	return (0);
}

/**
 * @brief  Opens the file for output redirection.
 * 
 * @param  node  Pointer to the output redirection node.
 * @return int   File descriptor, or -1 on failure.
 */
static int	ft_open_output_file(t_node *node)
{
	int	fd;

	if (ft_strcmp(node->token->value, ">>") == 0)
		fd = open(node->right->token->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(node->right->token->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd != -1)
		node->file = true;
	return (fd);
}

/**
 * @brief  Handles errors when opening the file.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return int Always returns 1 to indicate an error.
 */
static int	ft_handle_file_error(t_minishell *ms)
{
	perror("open");
	ft_set_exit_code(ms, 1);
	return (1);
}

/**
 * @brief  Handles errors when duplicating the file descriptor.
 * 
 * @param  fd  File descriptor.
 * @param  ms  Pointer to the minishell structure.
 * @return int Always returns 1 to indicate an error.
 */
static int	ft_handle_dup_error(int fd, t_minishell *ms)
{
	perror("dup2");
	close(fd);
	ft_set_exit_code(ms, 1);
	return (1);
}

// /**
//  * @brief  Handles input redirection from a file.
//  * 
//  * @param  node  Pointer to the input redirection node in the AST.
//  * @param  ms    Pointer to the minishell structure.
//  * @return int   Execution status.
//  **         0 on success.
//  **         Non-zero in case of errors or syntax issues.
//  */
// int	ft_handle_input_redirect(t_node *node, t_minishell *ms)
// {
// 	int	fd;

// 	if (!node->right) // Verifica se o token à direita é inválido
// 	{
// 		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
// 		ft_set_exit_code(ms, 2);
// 		return (1);
// 	}
// 	if (ft_invalid_right_token_value(node->right->token->value)) 	// Verifica se o token à direita é inválido
// 	{
// 		ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
// 		if (node->right->token->value)
// 			ft_putstr_fd(node->right->token->value, STDERR_FILENO);
// 		else
// 			ft_putstr_fd("newline", STDERR_FILENO);
// 		ft_putstr_fd("'\n", STDERR_FILENO);
// 		ft_set_exit_code(ms, 2);
// 		return (1);
// 	}	
// 	if (ft_is_valid_file(node->right->token->value, O_RDONLY)) 	// Verifica se o arquivo é inválido
// 	{
// 		ft_remove_created_files(node->prev);
// 		ft_create_files(node->left);
// 		ft_set_exit_code(ms, 1); // Checar se existem outputs_redirects para a frente em que os ficheiros precisem de ser criados!!
// 		return (1);
// 	}
// 	fd = open(node->right->token->value, O_RDONLY); // Abre o arquivo para leitura
// 	if (fd == -1)
// 	{
// 		perror("open");
// 		ft_set_exit_code(ms, 1);
// 		return (1);
// 	}
// 	if (dup2(fd, STDIN_FILENO) == -1) // Redireciona STDIN para o arquivo
// 	{
// 		perror("dup2");
// 		close(fd);
// 		ft_set_exit_code(ms, 1);
// 		return (1);
// 	}
// 	close(fd);
// 	ft_set_exit_code(ms, 0); // Redirecionamento bem-sucedido
// 	return (ft_execute_ast(node->left, ms));
// }

/**
 * @brief  Handles input redirection from a file.
 * 
 * @param  node  Pointer to the input redirection node in the AST.
 * @param  ms    Pointer to the minishell structure.
 * @return int   Execution status.
 *         0 on success.
 *         Non-zero in case of errors or syntax issues.
 */
int	ft_handle_input_redirect(t_node *node, t_minishell *ms)
{
	int	fd;

	if (ft_check_redirect_syntax(node, ms))
		return (1);
	if (ft_validate_input_file(node, ms))
		return (1);
	fd = open(node->right->token->value, O_RDONLY);
	if (fd == -1)
		return (ft_handle_file_error(ms));
	if (dup2(fd, STDIN_FILENO) == -1)
		return (ft_handle_dup_error(fd, ms));
	close(fd);
	ft_set_exit_code(ms, 0);
	return (ft_execute_ast(node->left, ms));
}

// /**
//  * @brief  Checks for syntax errors in input redirection.
//  * 
//  * @param  node  Pointer to the input redirection node.
//  * @param  ms    Pointer to the minishell structure.
//  * @return int   1 if there is a syntax error, 0 otherwise.
//  */
// static int	ft_check_input_redirect_syntax(t_node *node, t_minishell *ms)
// {
// 	if (!node->right)
// 	{
// 		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n",
// 			STDERR_FILENO);
// 		ft_set_exit_code(ms, 2);
// 		return (1);
// 	}
// 	if (ft_invalid_right_token_value(node->right->token->value))
// 	{
// 		ft_putstr_fd("minishell: syntax error near unexpected token `",
// 			STDERR_FILENO);
// 		if (node->right->token->value)
// 			ft_putstr_fd(node->right->token->value, STDERR_FILENO);
// 		else
// 			ft_putstr_fd("newline", STDERR_FILENO);
// 		ft_putstr_fd("'\n", STDERR_FILENO);
// 		ft_set_exit_code(ms, 2);
// 		return (1);
// 	}
// 	return (0);
// }

/**
 * @brief  Validates the input file before opening it.
 * 
 * @param  node  Pointer to the input redirection node.
 * @param  ms    Pointer to the minishell structure.
 * @return int   1 if the file is invalid, 0 otherwise.
 */
static int	ft_validate_input_file(t_node *node, t_minishell *ms)
{
	if (ft_is_valid_file(node->right->token->value, O_RDONLY))
	{
		ft_remove_created_files(node->prev);
		ft_create_files(node->left);
		ft_set_exit_code(ms, 1);
		return (1);
	}
	return (0);
}

/**
 * @brief  Checks if the command node contains the "cat" command.
 * 
 * @param  node  Pointer to the command node in the AST.
 * @return int   Boolean value indicating presence of "cat".
 **         1 if "cat" is found.
 **         0 otherwise.
 */
static int ft_has_cat(t_node *node) // FUNCAO DESENRASQUE ALTERAR
{
	t_node *current;

	current = node;
	if (!current)
		return (0);
	if (current->token->type == TKN_CMD)
	{
		if (!ft_strcmp(current->cmd_ready[0], "cat")
		|| !ft_strcmp(current->cmd_ready[0], "/bin/cat"))
			return (1);
	}
 	if (!current->left && !current->right)
		return (0);
	return (ft_has_cat(current->left));
	// ft_has_cat(current->right);
}

/**
 * @brief  Handles piping between commands in the AST.
 * 
 * @param  node  Pointer to the pipe node in the AST.
 * @param  ms    Pointer to the minishell structure.
 * @return int   Execution status.
 **         0 on success.
 **         Non-zero in case of errors or syntax issues.
 */
/* int	ft_handle_pipe(t_node *node, t_minishell *ms)
{
	char *input;
	char *temp;

	temp = NULL;
	if (!node->left)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
		ft_set_exit_code(ms, 2); // Código de erro para erro de sintaxe
		return (2);
	}
	else if (!node->right) // Caso de pipe sem lado direito
	{
		input = readline("> ");
		if (!input) // Ctrl-D
		{
			ft_putstr_fd("minishell: syntax error: unexpected end of file\n", STDERR_FILENO);
			ft_set_exit_code(ms, 258); // Código típico de erro de sintaxe
			return (258);
		}
		temp = ft_strjoin(ms->input, " ");
		free(ms->input);
		ms->input = ft_strjoin(temp, input);
		free(input);
		free(temp);
		ft_free_tokens(ms->tokens);
		ft_free_ast(ms->ast_root);
		ms->in_pipe = true;
		return (ft_process_input_and_execute(ms));
	}
	
	if (pipe(ms->pipefd) == -1)
	{
		perror("pipe");
		ft_set_exit_code(ms, 1);
		return (1);
	}
	ms->pid = fork();
	if (ms->pid == -1)
	{
		perror("fork");
		ft_set_exit_code(ms, 1);
		return (1);
	}
	if (ms->pid == 0)
	{
		close(ms->pipefd[0]);
		if (dup2(ms->pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(1);
		}
		close(ms->pipefd[1]);
		exit(ft_execute_ast(node->left, ms));
	}
	close(ms->pipefd[1]);
	if (!ft_has_cat(node))
		waitpid(ms->pid, &ms->status, 0); //  "$CASA" | '$HOME' | $USER $ | "$ HOME" << o     //  cat < test_files/infile_big | grep oi // [asa nso dois agora!]
	if (dup2(ms->pipefd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		ft_set_exit_code(ms, 1);
		close(ms->pipefd[0]);
		return (1);
	}
	close(ms->pipefd[0]);
	ft_set_exit_code(ms, 0);
	return (ft_execute_ast(node->right, ms));
}
 */

/**
 * @brief  Handles piping between commands.
 * 
 * @param  node  Pointer to the pipe node in the AST.
 * @param  ms    Pointer to the minishell structure.
 * @return int   Execution status.
 *         0 on success.
 *         Non-zero in case of errors or syntax issues.
 */
int	ft_handle_pipe(t_node *node, t_minishell *ms)
{
	if (ft_check_pipe_syntax(node, ms))
	{
		ft_set_exit_code(ms, 2);
		return (2);
	}
	if (ft_create_pipe(ms))
		return (1);
	ms->pid = fork();
	if (ms->pid == -1)
		return (ft_handle_fork_error(ms));
	if (ms->pid == 0)
		ft_execute_pipe_child(node, ms);
	close(ms->pipefd[1]);
	if (!ft_has_cat(node))
		waitpid(ms->pid, &ms->status, 0);
	if (ft_redirect_pipe_input(ms))
		return (1);
	return (ft_execute_ast(node->right, ms));
}

/**
 * @brief  Checks for syntax errors in pipe usage.
 * 
 * @param  node  Pointer to the pipe node.
 * @param  ms    Pointer to the minishell structure.
 * @return int   1 if there is a syntax error, 0 otherwise.
 */
static int	ft_check_pipe_syntax(t_node *node, t_minishell *ms)
{
	char	*input;

	if (!node->left)
		return (ft_pipe_syntax_error(ms, "|", 2));
	if (node->right)
		return (0);
	input = readline("> ");
	if (!input)
		return (ft_pipe_syntax_error(ms, "unexpected end of file", 258));
	ft_handle_unfinished_pipe(ms, input);
	ft_process_input_and_execute(ms);
	return (1);
}

/**
 * @brief  Prints a syntax error message for pipes and sets the exit code.
 * 
 * @param  ms     Pointer to the minishell structure.
 * @param  token  The unexpected token causing the error.
 * @param  code   Exit code to set.
 * @return int    Always returns 1.
 */
static int	ft_pipe_syntax_error(t_minishell *ms, char *token, int code)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	ft_putstr_fd(token, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	ft_set_exit_code(ms, code);
	return (1);
}

/**
 * @brief  Handles input continuation when the pipe is unfinished.
 * 
 * @param  ms     Pointer to the minishell structure.
 * @param  input  User input after an unfinished pipe.
 */
static void	ft_handle_unfinished_pipe(t_minishell *ms, char *input)
{
	char	*temp;

	temp = ft_strjoin(ms->input, " ");
	free(ms->input);
	ms->input = ft_strjoin(temp, input);
	free(input);
	free(temp);
	ft_free_tokens(ms->tokens);
	ft_free_ast(ms->ast_root);
	ms->in_pipe = true;
}

/**
 * @brief  Handles fork errors.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return int Always returns 1.
 */
static int	ft_handle_fork_error(t_minishell *ms)
{
	perror("fork");
	ft_set_exit_code(ms, 1);
	return (1);
}

/**
 * @brief  Executes the left side of the pipe in the child process.
 * 
 * @param  node  Pointer to the pipe node.
 * @param  ms    Pointer to the minishell structure.
 */
static void	ft_execute_pipe_child(t_node *node, t_minishell *ms)
{
	close(ms->pipefd[0]);
	if (dup2(ms->pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("dup2");
		exit(1);
	}
	close(ms->pipefd[1]);
	exit(ft_execute_ast(node->left, ms));
}

/**
 * @brief  Redirects pipe output to stdin for the next command.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return int 1 if an error occurred, 0 otherwise.
 */
static int	ft_redirect_pipe_input(t_minishell *ms)
{
	if (dup2(ms->pipefd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		ft_set_exit_code(ms, 1);
		close(ms->pipefd[0]);
		return (1);
	}
	close(ms->pipefd[0]);
	ft_set_exit_code(ms, 0);
	return (0);
}


/**
 * @brief  Executes a single command or builtin in a child process.
 * 
 * @param  node  Pointer to the command node in the AST.
 * @param  ms    Pointer to the minishell structure.
 * @return int   Exit code of the command.
 **         0 on success.
 **         Non-zero in case of error or if the command was not found.
 */
// int	ft_execute_command(t_node *node, t_minishell *ms)
// {
// 	int	valid;
// 	int sig;

// 	valid = -1;
// 	sig = 0;
// 	ms->pid = fork();
// 	if (ms->pid == -1)
// 	{
// 		perror("fork");
// 		ft_set_exit_code(ms, 1); // Código genérico para erro de fork
// 		return (1);
// 	}
// 	if (ms->pid == 0)
// 	{
// 		//ft_set_fork_signals();
// 		if (!node->cmd_ready[0] || node->cmd_ready[0][0] == '\0')
// 			exit(0);
// 		if (node->token->type == TKN_BLTIN) 
// 		{
// 			if (ft_strcmp(node->token->value, "echo"))
// 				exit(ft_exec_builtins_check(node, ms));
// 			else
// 				exit(ft_exec_builtins(node, ms));
// 		}
// 		if (node->cmd_ready[0][0] == '/' || 									// Caminho absoluto ou relativo
// 			(node->cmd_ready[0][0] == '.' && node->cmd_ready[0][1] == '/') || 
// 			!ft_strncmp(node->cmd_ready[0], "../", 3)) 
// 		{
// 			valid = ft_is_valid_file(node->cmd_ready[0], X_OK); // Verifica se o arquivo é válido
// 			if (valid != 0)
// 				exit(valid); // Retorna o erro correspondente (127 ou 126)
// 			execve(node->cmd_ready[0], node->cmd_ready, ms->env.envp); // Executa diretamente se for válido
// 			// ft_putstr_fd("minishell: ", STDERR_FILENO);
// 			// ft_putstr_fd(node->cmd_ready[0], STDERR_FILENO);
// 			// ft_putstr_fd(": command not found\n", STDERR_FILENO);
// 			exit(42);
// 		}
// 		if (ft_find_executable(ms, node->cmd_ready[0]) == 127)
// 		{
// /* 			ft_putstr_fd(node->cmd_ready[0], STDERR_FILENO);
// 			ft_putstr_fd(": command not found\n", STDERR_FILENO); */
// 			exit(42);
// 		}
// 		if (node->cmd_ready[1] == NULL && 
// 		!ft_strcmp(node->cmd_ready[0], "cat") && node->prev && node->prev->token->type == TKN_PIPE
// 		&& (node->prev->left == node 
// 		|| (node->prev->prev && node->prev->prev->token->type == TKN_PIPE && node->prev->right == node)))
// 			exit(13);
// 		execve(ms->env.full_path, node->cmd_ready, ms->env.envp); // Executa o executável encontrado
// 		perror("execve");
// 		exit(127);
// 	}
// 	ft_set_fork_signals();
// 	waitpid(ms->pid, &ms->status, 0);
// 	if (WIFEXITED(ms->status)) // Processo terminou normalmente
// 	{
// 		ft_set_exit_code(ms, WEXITSTATUS(ms->status));
//  		if (ft_exit_code(ms) == 42 && node->cmd_ready[0])
// 		{
// 			ft_putstr_fd(node->cmd_ready[0], STDERR_FILENO);
// 			ft_putstr_fd(": command not found\n", STDERR_FILENO); // ou Command '' not found
// 			ft_set_exit_code(ms, 127);
// 		}
// 		else if (ft_exit_code(ms) == 0)
// 		{
// 			if (node->token->type == TKN_BLTIN && ft_strcmp(node->token->value, "echo"))
// 				ft_exec_builtins(node, ms);
// 		}
// 	}
// 	else if (WIFSIGNALED(ms->status)) // Processo foi terminado por um sinal
// 	{
// 		sig = WTERMSIG(ms->status);
// 		ft_set_exit_code(ms, 128 + sig);
// 		if (sig == SIGINT)
// 			write(STDERR_FILENO, "\n", 1);
// 	}
// 	else
// 		ft_set_exit_code(ms, 1);
// 	ft_set_main_signals();
// 	if (ft_exit_code(ms) != 0 && node->prev && node->prev->token->type == TKN_IN_RD) // Remoção de arquivos criados caso o comando falhe
// 	{
// 		ft_remove_created_files(node->prev);
// 	}
// 	if (node->token->type == TKN_BLTIN && !ft_strcmp(node->cmd_ready[0], "exit") && ft_exit_code(ms) != 1) // Finaliza o shell se for exit
// 	{
// 		ft_free_tokens(ms->tokens);
// 		ft_free_ast(ms->ast_root);
// 		free(ms->input);
// 		exit(ft_exit_code(ms));
// 	}
// 	return (ft_exit_code(ms));
// }

/**
 * @brief  Handles the execution of built-in commands.
 * 
 * @param  node  Pointer to the AST node containing the command.
 * @param  ms    Pointer to the minishell structure.
 */
static void	ft_execute_builtin(t_node *node, t_minishell *ms)
{
	if (ft_strcmp(node->token->value, "echo"))
		exit(ft_exec_builtins_check(node, ms));
	exit(ft_exec_builtins(node, ms));
}

/**
 * @brief  Checks if the command is a valid executable.
 * 
 * @param  node  Pointer to the AST node containing the command.
 * @param  ms    Pointer to the minishell structure.
 * @return int   Returns execution status.
 */
static int	ft_execute_external(t_node *node, t_minishell *ms)
{
	int	valid;

	valid = ft_is_valid_file(node->cmd_ready[0], X_OK);
	if (valid != 0)
		exit(valid);
	execve(node->cmd_ready[0], node->cmd_ready, ms->env.envp);
	exit(42);
}

/**
 * @brief  Executes the command in a child process.
 * 
 * @param  node  Pointer to the AST node containing the command.
 * @param  ms    Pointer to the minishell structure.
 */
static void	ft_execute_child_process(t_node *node, t_minishell *ms)
{
	if (!node->cmd_ready[0] || node->cmd_ready[0][0] == '\0')
		exit(0);
	if (node->token->type == TKN_BLTIN)
		ft_execute_builtin(node, ms);
	if (node->cmd_ready[0][0] == '/' || node->cmd_ready[0][0] == '.' ||
		!ft_strncmp(node->cmd_ready[0], "../", 3))
		ft_execute_external(node, ms);
	if (ft_find_executable(ms, node->cmd_ready[0]) == 127)
		exit(42);
	if (node->cmd_ready[1] == NULL && !ft_strcmp(node->cmd_ready[0], "cat") &&
		node->prev && node->prev->token->type == TKN_PIPE &&
		(node->prev->left == node ||
			(node->prev->prev && node->prev->prev->token->type == TKN_PIPE &&
				node->prev->right == node)))
		exit(13);
	execve(ms->env.full_path, node->cmd_ready, ms->env.envp);
	perror("execve");
	exit(127);
}

/**
 * @brief  Handles the termination of a process and sets the exit code.
 * 
 * @param  node  Pointer to the AST node.
 * @param  ms    Pointer to the minishell structure.
 */
static void	ft_handle_cmd_exit_status(t_node *node, t_minishell *ms)
{
	int	sig;

	if (WIFEXITED(ms->status))
	{
		ft_set_exit_code(ms, WEXITSTATUS(ms->status));
		if (ft_exit_code(ms) == 42 && node->cmd_ready[0])
		{
			ft_putstr_fd(node->cmd_ready[0], STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			ft_set_exit_code(ms, 127);
		}
		else if (ft_exit_code(ms) == 0 && node->token->type == TKN_BLTIN &&
				 ft_strcmp(node->token->value, "echo"))
			ft_exec_builtins(node, ms);
	}
	else if (WIFSIGNALED(ms->status))
	{
		sig = WTERMSIG(ms->status);
		ft_set_exit_code(ms, 128 + sig);
		if (sig == SIGINT)
			write(STDERR_FILENO, "\n", 1);
	}
	else
		ft_set_exit_code(ms, 1);
}

/**
 * @brief  Executes a command node in the AST.
 * 
 * @param  node  Pointer to the AST node containing the command.
 * @param  ms    Pointer to the minishell structure.
 * @return int   Exit status of the command.
 */
int	ft_execute_command(t_node *node, t_minishell *ms)
{
	ms->pid = fork();
	if (ms->pid == -1)
		return (ft_handle_fork_error(ms));
	if (ms->pid == 0)
		ft_execute_child_process(node, ms);
	ft_set_fork_signals();
	waitpid(ms->pid, &ms->status, 0);
	ft_handle_cmd_exit_status(node, ms);
	ft_set_main_signals();
	if (ft_exit_code(ms) != 0 && node->prev &&
		node->prev->token->type == TKN_IN_RD)
		ft_remove_created_files(node->prev);
	if (node->token->type == TKN_BLTIN &&
		!ft_strcmp(node->cmd_ready[0], "exit") &&
		ft_exit_code(ms) != 1)
	{
		ft_free_tokens(ms->tokens);
		ft_free_ast(ms->ast_root);
		free(ms->input);
		exit(ft_exit_code(ms));
	}
	return (ft_exit_code(ms));
}


/**
 * @brief  Removes files created during the execution of output redirection nodes.
 * 
 * @param  node  Pointer to the node in the AST containing the output redirection.
 */
void	ft_remove_created_files(t_node *node)
{
	if (!node)
		return ;
	if (node->token->type == TKN_OUT_RD && node->right && node->right->token->value && node->file == true) // Se for um OUTPUT_REDIRECT, tenta remover o arquivo associado
	{
		if (unlink(node->right->token->value) == -1)
			perror("unlink");
		else
			node->file = false;
	}
	if (node->prev && node->prev->right == node) // Se estivermos em um ramo direito, seguimos para o nó anterior diretamente
	{
		ft_remove_created_files(node->prev);
		return ;
	}
	if (node->prev && node->prev->right) // Se estivermos em um ramo esquerdo, verificamos o ramo direito do nó anterior
		ft_remove_created_files(node->prev->right);
	if (node->prev) // Continua a recursão para o nó anterior
		ft_remove_created_files(node->prev);
}

/**
 * @brief  Creates files for output redirection nodes if they don't already exist.
 * 
 * @param  node  Pointer to the node in the AST containing the output redirection.
 */
void	ft_create_files(t_node *node)
{
	int fd;

	if (!node)
		return;
	if (node->token->type == TKN_OUT_RD && node->right && node->right->token->value && node->file == false)
	{
		if (ft_strcmp(node->token->value, ">>") == 0)
			fd = open(node->right->token->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(node->right->token->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd != -1)
			close(fd);
		else
			perror("open");
	}
	if (node->left && node->left->token->type == TKN_OUT_RD) // Se o nó à esquerda também for um output redirect, continuar a verificação
		ft_create_files(node->left);
}

/**
 * @brief  Searches for an executable file in the system PATH.
 * 
 * @param  ms   Pointer to the minishell structure containing environment variables.
 * @param  cmd  Command to search for in the PATH.
 * @return int  Status of the search.
 **         0 if the executable is found.
 **         127 if not found or if PATH is invalid.
 */
int	ft_find_executable(t_minishell *ms, char *cmd)
{
	int	i;

	i = 0;
	ms->env.env_paths = getenv("PATH");
	if (!ms->env.env_paths)
		return (127); // PATH não encontrado
	ms->env.paths = ft_split(ms->env.env_paths, ':');
	if (!ms->env.paths)
		return (127); // Falha ao dividir o PATH
	while (ms->env.paths[i])
	{
		ms->env.full_path = malloc(ft_strlen(ms->env.paths[i]) + ft_strlen(cmd) + 2);
		if (!ms->env.full_path)
		{
			ft_free_split(ms->env.paths);
			return (127);
		}
		ft_strcpy(ms->env.full_path, ms->env.paths[i]);
		ft_strcat(ms->env.full_path, "/");
		ft_strcat(ms->env.full_path, cmd);
		if (access(ms->env.full_path, X_OK) == 0)
		{
			ft_free_split(ms->env.paths);
			return (0); // Comando encontrado
		}
		free(ms->env.full_path);
		i++;
	}
	ft_free_split(ms->env.paths);
	return (127); // Comando não encontrado
}

/**
 * @brief  Checks if the token value on the right side is invalid for redirection or other syntax.
 * 
 * @param  value  Token value to check.
 * @return int    Boolean indicating validity.
 **         1 if invalid.
 **         0 if valid.
 */
int	ft_invalid_right_token_value(char *value)
{
	if (!value || 
		ft_strcmp(value, ">") == 0 || ft_strcmp(value, ">>") == 0 || 
		ft_strcmp(value, "<") == 0 || ft_strcmp(value, "<<") == 0 || 
		ft_strcmp(value, "|") == 0 || ft_strcmp(value, "&&") == 0 || 
		ft_strcmp(value, "||") == 0)
		return (1);
	return (0);
}

// /**
//  * @brief  Checks if the given file is valid and accessible for the specified mode.
//  * 
//  * @param  filepath  Path to the file.
//  * @param  mode      Access mode to check (e.g., read, write, execute).
//  * @return int       Status code.
//  **         0 if the file is valid.
//  **         127 if the file does not exist.
//  **         126 if it is a directory or lacks necessary permissions.
//  */
// int	ft_is_valid_file(char *filepath, int mode)
// {
// 	struct stat	file_stat;
	
// 	if (!filepath) // Verifica se o caminho é válido
// 	{
// 		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
// 		return (1);
// 	}
// 	if (stat(filepath, &file_stat) == -1) // Verifica se o arquivo existe
// 	{
// 		ft_putstr_fd("minishell: ", STDERR_FILENO);
// 		ft_putstr_fd(filepath, STDERR_FILENO);
// 		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
// 		return (127); // Código de erro para arquivo inexistente
// 	}
// 	if (S_ISDIR(file_stat.st_mode)) 	// Verifica se é um diretório
// 	{
// 		ft_putstr_fd("minishell: ", STDERR_FILENO);
// 		ft_putstr_fd(filepath, STDERR_FILENO);
// 		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
// 		return (126); // Código de erro para diretório
// 	}
// 	if (mode == X_OK) // Verifica permissões de execução
// 	{
// 		if (access(filepath, X_OK) == -1)
// 		{
// 			ft_putstr_fd("minishell: ", STDERR_FILENO);
// 			ft_putstr_fd(filepath, STDERR_FILENO);
// 			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
// 			return (126); // Código de erro para permissão negada
// 		}
// 		return (0);
// 	}
// 	if (mode == O_RDONLY) 	// Verifica permissões de leitura
// 	{
// 		if (access(filepath, R_OK) == -1)
// 		{
// 			ft_putstr_fd("minishell: ", STDERR_FILENO);
// 			ft_putstr_fd(filepath, STDERR_FILENO);
// 			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
// 			return (126); // Código de erro para permissão negada
// 		}
// 		return (0);
// 	}
// 	if (mode == O_WRONLY || mode == (O_WRONLY | O_CREAT) || mode == (O_WRONLY | O_APPEND)) // Escrita
// 	{
// 		if (access(filepath, W_OK) == -1)
// 		{
// 			if (errno != ENOENT) // Arquivo existe, mas sem permissão de escrita
// 			{
// 				ft_putstr_fd("minishell: ", STDERR_FILENO);
// 				ft_putstr_fd(filepath, STDERR_FILENO);
// 				ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
// 				return (126);
// 			}
// 		}	
// 	}
// 	return (0); // Arquivo válido
// }

/**
 * @brief  Checks if the given file is valid and accessible for the specified mode.
 * 
 * @param  filepath  Path to the file.
 * @param  mode      Access mode to check (e.g., read, write, execute).
 * @return int       Status code.
 **         0 if the file is valid.
 **         127 if the file does not exist.
 **         126 if it is a directory or lacks necessary permissions.
 */
int	ft_is_valid_file(char *filepath, int mode)
{
	struct stat	file_stat;

	if (!filepath)
		return (ft_file_error(NULL, "syntax error near unexpected token `newline'", 1));
	if (stat(filepath, &file_stat) == -1)
		return (ft_file_error(filepath, "No such file or directory", 127));
	if (S_ISDIR(file_stat.st_mode))
		return (ft_file_error(filepath, "Is a directory", 126));
	return (ft_check_file_access(filepath, mode));
}

/**
 * @brief  Handles file-related error messages and returns the appropriate error code.
 * 
 * @param  filepath  Path to the file.
 * @param  msg       Error message to display.
 * @param  code      Error code to return.
 * @return int       Always returns the provided error code.
 */
static int	ft_file_error(char *filepath, char *msg, int code)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (filepath)
		ft_putstr_fd(filepath, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (code);
}

/**
 * @brief  Checks the file's access permissions based on the requested mode.
 * 
 * @param  filepath  Path to the file.
 * @param  mode      Access mode to check.
 * @return int       Status code (0 if valid, 126 if permission denied).
 */
static int	ft_check_file_access(char *filepath, int mode)
{
	if (mode == X_OK)
	{
		if (access(filepath, X_OK) == -1)
			return (ft_file_error(filepath, "Permission denied", 126));
		return (0);
	}
	if (mode == O_RDONLY)
	{
		if (access(filepath, R_OK) == -1)
			return (ft_file_error(filepath, "Permission denied", 126));
		return (0);
	}
	if (mode == O_WRONLY || mode == (O_WRONLY | O_CREAT) || mode == (O_WRONLY | O_APPEND))
	{
		if (access(filepath, W_OK) == -1 && errno != ENOENT)
			return (ft_file_error(filepath, "Permission denied", 126));
	}
	return (0);
}

// int ft_handle_multiple_heredocs(t_node *node, t_minishell *ms)
// {
//     char *input;
//     int save_stdout;
//     int status;
//     t_node *current;
//     int i;
//     char *new_temp;

//     save_stdout = -1;
//     ms->temp = NULL;
//     i = 0;
//     new_temp = NULL;
//     if (!node || !node->heredoc_stops)
//         return (1);
//     if (!isatty(STDOUT_FILENO)) // Salvar STDOUT se necessário
//     {
//         save_stdout = dup(STDOUT_FILENO);
//         if (save_stdout == -1)
//         {
//             perror("dup");
//             return (1);
//         }
//     }
//     while (node->heredoc_stops[i]) // Processar todos os heredocs
//     {
//         if (g_interrupt)
//         {
//             ft_set_exit_code(ms, 130);
//             break;
//         }
//         if (pipe(ms->pipefd) == -1) // Criar o pipe para o heredoc
//         {
//             perror("pipe");
//             return (1);
//         }
//         ms->pid = fork();
//         if (ms->pid == -1)
//         {
//             perror("fork");
//             close(ms->pipefd[0]);
//             close(ms->pipefd[1]);
//             return (1);
//         }
//         ft_set_fork_signals();
//         if (ms->pid == 0)
//         {
//             ft_set_heredoc_signals();
//             close(ms->pipefd[0]);
//             ms->temp = NULL;
//             while (!g_interrupt)
//             {
//                 input = readline("> ");
//                 if (g_interrupt)
//                     exit(130);
//                 if (!input)
//                 {
//                     ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted ", STDERR_FILENO);
//                     ft_putstr_fd(node->heredoc_stops[i], STDERR_FILENO);
//                     write(STDERR_FILENO, "')\n", 3);
//                     break;
//                 }
//                 if (ft_strcmp(input, node->heredoc_stops[i]) == 0)
//                     break;
//                 if (ms->temp == NULL)
//                     ms->temp = ft_strdup(input);
//                 else
//                 {
//                     new_temp = ft_strjoin_free(ms->temp, "\n", 1, 0);
//                     ms->temp = ft_strjoin_free(new_temp, input, 1, 0);
//                 }
//                 free(input);
//             }
//             if (ms->temp && ms->c_multi_heredocs == i + 1)
//             {
//                 new_temp = ft_strjoin_free(ms->temp, "\n", 1, 0);
// 				ft_revalue_heredock_input(&new_temp, ms);
//                 ft_putstr_fd(new_temp, ms->pipefd[1]);
//                 free(new_temp);
//             }
//             close(ms->pipefd[1]);
//             exit(0);
//         }
//         close(ms->pipefd[1]);
//         waitpid(ms->pid, &status, 0);
//         if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
//         {
//             g_interrupt = 1;
//             ft_set_exit_code(ms, 130);
//             close(ms->pipefd[0]);
//             break;
//         }
//         if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
//         {
//             ft_set_exit_code(ms, 130);
//             close(ms->pipefd[0]);
//             break;
//         }
//         i++;
//     }
//     if (!WIFSIGNALED(status) && WEXITSTATUS(status) != 130) // Após todos os heredocs, redirecionar o stdin do processo pai para o pipe
//     {
//         if (dup2(ms->pipefd[0], STDIN_FILENO) == -1 && !g_interrupt)
//         {
//             perror("dup2 aqui");
//             close(ms->pipefd[0]);
//             ft_set_exit_code(ms, 1);
//             return (1);
//         }
//     }
//     close(ms->pipefd[0]);
//     ft_set_main_signals();
//     if (save_stdout != -1) // Restaurar STDOUT se necessário
//     {
//         if (dup2(save_stdout, STDOUT_FILENO) == -1)
//         {
//             perror("dup2 este");
//             close(save_stdout);
//             ft_set_exit_code(ms, 1);
//             return (1);
//         }
//         close(save_stdout);
//     }
//     current = node->left;
//     while (current && current->token->type == TKN_HDOC)
//     {
//         t_node *temp_node = current;
//         current = current->left;
//         free(temp_node);
//     }
//     node->left = current;
//     g_interrupt = 0;
//     return (ft_execute_ast(node->left, ms));
// }

/**
 * @brief  Handles multiple heredocs consecutively and passes the data.
 * 
 * @param  node  Pointer to the AST node containing multiple heredocs.
 * @param  ms    Pointer to the minishell structure.
 * @return int   Execution status.
 **         0 on success.
 **         Non-zero in case of errors or interruptions.
 */
int	ft_handle_multiple_heredocs(t_node *node, t_minishell *ms)
{
	int		save_stdout;
	int		i;
	int		pipe_status;

	save_stdout = -1;
	ms->temp = NULL;
	i = 0;
	if (!node || !node->heredoc_stops)
		return (1);
	if (!node->right)
		return (ft_heredoc_syntax_error(ms));
	if (!isatty(STDOUT_FILENO))
		save_stdout = ft_redirect_stdout(ms);
	while (node->heredoc_stops[i])
	{
		pipe_status = ft_create_pipe(ms);
		if (pipe_status)
			return (pipe_status);
		if (ft_fork_heredoc(ms, node, &i))
			break ;
		i++;
	}
	ft_cleanup_heredocs(ms, save_stdout, node);
	return (ft_handle_exit_status(ms, node));
}

/**
 * @brief  Creates a fork for processing a single heredoc.
 * 
 * @param  ms    Pointer to the minishell structure.
 * @param  node  Pointer to the current AST node.
 * @param  i     Pointer to heredoc index.
 * @return int   1 if interrupted, 0 otherwise.
 */
static int	ft_fork_heredoc(t_minishell *ms, t_node *node, int *i)
{
	ms->pid = fork();
	if (ms->pid == -1)
		return (ft_handle_heredoc_fork_error(ms));
	ft_set_fork_signals();
	if (ms->pid == 0)
		ft_multiple_heredoc_child(node, ms, i);
	close(ms->pipefd[1]);
	waitpid(ms->pid, &ms->status, 0);
	ft_set_main_signals();
	if (WIFEXITED(ms->status) && WEXITSTATUS(ms->status) == 130)
		return (1);
	return (0);
}

/**
 * @brief  Cleans up heredoc-related processes and restores state.
 * 
 * @param  ms         Pointer to the minishell structure.
 * @param  save_stdout Saved stdout descriptor.
 * @param  node       Pointer to the current AST node.
 */
static void	ft_cleanup_heredocs(t_minishell *ms, int save_stdout, t_node *node)
{
	t_node	*current;
	t_node	*temp_node;

	temp_node = NULL;
	if (WEXITSTATUS(ms->status) != 130)
		ft_restore_stdin(ms);
	close(ms->pipefd[0]);
	if (save_stdout != -1)
		ft_restore_stdout(save_stdout, ms);
	current = node->left;
	while (current && current->token->type == TKN_HDOC)
	{
		temp_node = current;
		current = current->left;
		free(temp_node);
	}
	node->left = current;
	node->left->prev = node;
}

/**
 * @brief  Handles heredoc input for a child process.
 * 
 * @param  node  Pointer to the AST node containing heredoc.
 * @param  ms    Pointer to the minishell structure.
 * @param  i     Pointer to heredoc index.
 */
static void	ft_multiple_heredoc_child(t_node *node, t_minishell *ms, int *i)
{
	char	*input;

	input = NULL;
	ft_set_heredoc_signals();
	close(ms->pipefd[0]);
	while (!g_interrupt)
	{
		input = readline("> ");
		if (!input)
		{
			ft_putstr_fd("minishell: warning: here-document delimited by EOF"
				" (wanted `", STDERR_FILENO);
			ft_putstr_fd(node->heredoc_stops[*i], STDERR_FILENO);
			write(STDERR_FILENO, "')\n", 3);
			break ;
		}
		if (ft_process_heredoc_input(node, ms, input, i))
			break ;
		free(input);
	}
	ft_finalize_heredoc(ms, i);
}

/**
 * @brief  Processes heredoc input and checks for termination.
 * 
 * @param  node   Pointer to the AST node.
 * @param  ms     Pointer to the minishell structure.
 * @param  input  User input from readline.
 * @param  i      Pointer to heredoc index.
 * @return int    1 if heredoc should stop, 0 otherwise.
 */
static int	ft_process_heredoc_input(t_node *node, t_minishell *ms,
								char *input, int *i)
{
	char	*new_temp;

	if (ft_strcmp(input, node->heredoc_stops[*i]) == 0)
		return (1);
	if (ms->temp == NULL)
		ms->temp = ft_strdup(input);
	else
	{
		new_temp = ft_strjoin_free(ms->temp, "\n", 1, 0);
		ms->temp = ft_strjoin_free(new_temp, input, 1, 0);
	}
	return (0);
}

/**
 * @brief  Finalizes heredoc processing and writes to pipe if needed.
 * 
 * @param  ms    Pointer to the minishell structure.
 * @param  node  Pointer to the AST node.
 * @param  i     Pointer to heredoc index.
 */
static void	ft_finalize_heredoc(t_minishell *ms, int *i)
{
	char	*new_temp;

	if (ms->temp && ms->c_multi_heredocs == *i + 1)
	{
		new_temp = ft_strjoin_free(ms->temp, "\n", 1, 0);
		ft_revalue_heredock_input(&new_temp, ms);
		ft_putstr_fd(new_temp, ms->pipefd[1]);
		free(new_temp);
	}
	close(ms->pipefd[1]);
	exit(0);
}

// /**
//  * @brief  Collects all heredocs from the current node and its left branch.
//  * 
//  * @param  node  Pointer to the starting heredoc node in the AST.
//  * @param  ms    Pointer to the minishell structure.
//  * @return int   Status of the collection.
//  **         0 on success.
//  **         Non-zero in case of syntax errors.
//  */
// int	ft_collect_heredocs(t_node *node, t_minishell *ms)
// {
// 	t_node	*current;
// 	int		i;

// 	i = 0;
// 	ms->c_multi_heredocs = 0;
// 	if (!node || node->token->type != TKN_HDOC)
// 		return (0);
	
// 	// Contar heredocs consecutivos
// 	current = node;
// 	while (current && current->token->type == TKN_HDOC)
// 	{
// 		ms->c_multi_heredocs++;
// 		current = current->left;
// 	}

// 	// Alocar espaço para os stop tokens
// 	node->heredoc_stops = malloc(sizeof(char *) * (ms->c_multi_heredocs + 1));
// 	if (!node->heredoc_stops)
// 		return (0);

// 	// Preencher os stop tokens do último para o primeiro
// 	current = node;
// 	i = ms->c_multi_heredocs;
// 	while (current && --i >= 0)
// 	{
// 		if(!current->right)
// 		{
// 			ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
// 			//ft_putstr_fd(current->left->token->value, STDERR_FILENO);
// 			ft_putchar_fd(current->left->token->value[0], STDERR_FILENO);
// 			ft_putstr_fd("'\n", STDERR_FILENO);
// 			ft_set_exit_code(ms, 2);
// 			return (1);
// 		}
// 		node->heredoc_stops[i] = current->right->token->value;
// 		current = current->left;
// 	}
// 	node->heredoc_stops[ms->c_multi_heredocs] = NULL; // Terminar com NULL
// 	return (0);
// }

/**
 * @brief  Handles syntax errors in heredoc collection.
 * 
 * @param  current  Pointer to the current node in the AST.
 * @param  ms       Pointer to the minishell structure.
 * @return int      Returns 1 to indicate an error.
 */
static int	ft_multiple_heredoc_syntax_error(t_node *current, t_minishell *ms)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	ft_putchar_fd(current->left->token->value[0], STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	ft_set_exit_code(ms, 2);
	return (1);
}

/**
 * @brief  Allocates and populates heredoc stop tokens.
 * 
 * @param  node  Pointer to the starting heredoc node in the AST.
 * @param  ms    Pointer to the minishell structure.
 * @return int   Returns 0 on success, 1 on error.
 */
static int	ft_allocate_heredoc_stops(t_node *node, t_minishell *ms)
{
	t_node	*current;
	int		i;

	node->heredoc_stops = malloc(sizeof(char *) * (ms->c_multi_heredocs + 1));
	if (!node->heredoc_stops)
		return (0);
	current = node;
	i = ms->c_multi_heredocs;
	while (current && --i >= 0)
	{
		if (!current->right)
			return (ft_multiple_heredoc_syntax_error(current, ms));
		node->heredoc_stops[i] = current->right->token->value;
		current = current->left;
	}
	node->heredoc_stops[ms->c_multi_heredocs] = NULL;
	return (0);
}

/**
 * @brief  Collects all heredocs from the current node and its left branch.
 * 
 * @param  node  Pointer to the starting heredoc node in the AST.
 * @param  ms    Pointer to the minishell structure.
 * @return int   Status of the collection.
 **         0 on success.
 **         Non-zero in case of syntax errors.
 */
int	ft_collect_heredocs(t_node *node, t_minishell *ms)
{
	t_node	*current;

	ms->c_multi_heredocs = 0;
	if (!node || node->token->type != TKN_HDOC)
		return (0);
	current = node;
	while (current && current->token->type == TKN_HDOC)
	{
		ms->c_multi_heredocs++;
		current = current->left;
	}
	return (ft_allocate_heredoc_stops(node, ms));
}

