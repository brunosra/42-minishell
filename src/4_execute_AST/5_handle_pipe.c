/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5_handle_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 02:35:12 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/03/09 08:34:17 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int			ft_handle_pipe(t_node *node, t_minishell *ms);
static int	ft_redirect_pipe_input(t_minishell *ms);
static void	ft_execute_pipe_child(t_node *node, t_minishell *ms);
static void	ft_execute_pipe_child2(t_node *node, t_minishell *ms);

/**
 * @brief  Waits for child processes and updates exit code accordingly.
 * 
 * @param  ms      Pointer to the minishell structure.
 * @param  pid1    PID of the first child process.
 * @param  pid2    PID of the second child process.
 */
static void	ft_wait_for_pipe_children(t_minishell *ms, pid_t pid1, pid_t pid2)
{
	int	status1;
	int	status2;
	int	pid_terminated;

	status1 = -1;
	status2 = -1;
	pid_terminated = waitpid(-1, &status1, 0);
	if (pid_terminated == pid2)
	{
		status2 = WEXITSTATUS(status1);
		kill(pid1, SIGTERM);
		waitpid(pid1, NULL, 0);
		ft_exit_code(status2);
	}
	else if (pid_terminated == pid1)
	{
		waitpid(pid2, &status2, 0);
		ft_exit_code(WEXITSTATUS(status2));
	}
}

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
	pid_t	pid2;

	if (ft_check_pipe_syntax(node, ms))
		return (ft_exit_code(2));
	if (ft_create_pipe(ms))
		return (1);
	ms->pid = fork();
	if (ms->pid == -1)
		return (ft_handle_fork_error());
	ft_set_fork_signals();
	if (ms->pid == 0)
		ft_execute_pipe_child(node, ms);
	close(ms->pipefd[1]);
	ft_set_main_signals();
	if (ft_redirect_pipe_input(ms))
		return (1);
	pid2 = fork();
	if (pid2 == -1)
		return (ft_handle_fork_error());
	if (pid2 == 0)
		ft_execute_pipe_child2(node->right, ms);
	ft_wait_for_pipe_children(ms, ms->pid, pid2);
	return (ft_exit_code(-1));
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
		ft_exit_code(1);
		close(ms->pipefd[0]);
		return (1);
	}
	close(ms->pipefd[0]);
	ft_exit_code(0);
	return (0);
}

/**
 * @brief  Executes the left side of the pipe in the child process.
 * 
 * @param  node  Pointer to the pipe node.
 * @param  ms    Pointer to the minishell structure.
 */
static void	ft_execute_pipe_child(t_node *node, t_minishell *ms)
{
	ft_set_heredoc_signals();
	close(ms->pipefd[0]);
	if (dup2(ms->pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("dup2");
		exit(1);
	}
	close(ms->pipefd[1]);
	ft_execute_ast(node->left, ms);
	exit(ft_free_ms(ms, true, true, ft_exit_code(-1)));
}

/**
 * @brief  Executes the right side of a pipe in a separate child process.
 * 
 * @param  node  Pointer to the right-side command node in the AST.
 * @param  ms    Pointer to the minishell structure.
 * 
 * The function executes the command, frees all allocated memory,
 * and exits with the appropriate status code.
 */
void	ft_execute_pipe_child2(t_node *node, t_minishell *ms)
{
	ft_execute_ast(node->right, ms);
	ft_free_ms(ms, true, true, 0);
	exit(ft_exit_code(-1));
}
