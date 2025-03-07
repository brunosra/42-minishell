/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5_handle_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 02:35:12 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/03/07 02:07:18 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int			ft_handle_pipe(t_node *node, t_minishell *ms);
static int	ft_redirect_pipe_input(t_minishell *ms);
static void	ft_execute_pipe_child(t_node *node, t_minishell *ms);
int			ft_handle_fork_error(void);

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
		ft_exit_code(2);
		return (2);
	}
	if (ft_create_pipe(ms))
		return (1);
	ms->pid = fork();
	if (ms->pid == -1)
		return (ft_handle_fork_error());
	ft_set_fork_signals();
	if (ms->pid == 0)
		ft_execute_pipe_child(node, ms);
	close(ms->pipefd[1]);
	if (!ft_has_cat(node))
		waitpid(ms->pid, &ms->status, 0);
	ft_set_main_signals();
	if (ft_redirect_pipe_input(ms))
		return (1);
	return (ft_execute_ast(node->right, ms));
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
	close(ms->pipefd[0]);
	if (dup2(ms->pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("dup2");
		exit(1);
	}
	close(ms->pipefd[1]);
	ft_execute_ast(node->left, ms);
	exit(ft_free_ms(ms, true, true, 0));
}

/**
 * @brief  Handles fork errors.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return int Always returns 1.
 */
int	ft_handle_fork_error(void)
{
	perror("fork");
	ft_exit_code(1);
	return (1);
}
