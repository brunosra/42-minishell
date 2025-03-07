/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_finalize_execution.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 02:22:33 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/03/07 08:03:13 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void		ft_finalize_execution(t_minishell *ms);
static void	ft_close_stdin_stdout(t_minishell *ms);
static void	ft_clean_stuck_cats(t_minishell *ms);
static void	ft_clean_stuck_cats_child(t_minishell *ms);
void		ft_find_stuck_cats(t_minishell *ms, t_node *node);

/**
 * @brief  Finalizes the execution process.
 * 
 * Ensures AST execution, restores input/output, handles pipes, and
 * cleans up stuck "cat" processes if necessary.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return void
 */
void	ft_finalize_execution(t_minishell *ms)
{
	ms->status = ft_execute_ast(ms->ast_root, ms);
	if ((dup2(ms->save_stdin, STDIN_FILENO) == -1
			|| dup2(ms->save_stdout, STDOUT_FILENO) == -1)
		&& ms->in_pipe == false)
	{
		ft_close_stdin_stdout(ms);
		ft_perror("dup2", 1);
		return ;
	}
	if (ms->in_pipe == true)
		ms->in_pipe = false;
	else
		ft_close_stdin_stdout(ms);
	if (ms->status != 130)
		ft_clean_stuck_cats(ms);
}

/**
 * @brief  Close saved stdin and stdout file descriptors.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return void
 */
static void	ft_close_stdin_stdout(t_minishell *ms)
{
	close(ms->save_stdin);
	close(ms->save_stdout);
}

/**
 * @brief  Handles stuck "cat" commands waiting for input.
 * 
 * Spawns a child process to handle stuck "cat" commands, allowing input
 * until the process is interrupted or completed.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return void
 */
void	ft_clean_stuck_cats(t_minishell *ms)
{
	if (!ms->c_stuck_cats)
		return ;
	ft_create_pipe(ms);
	ms->pid = fork();
	if (ms->pid == -1)
		return ;
	ft_set_fork_signals();
	if (ms->pid == 0)
		ft_clean_stuck_cats_child(ms);
	close(ms->pipefd[1]);
	waitpid(ms->pid, &ms->status, 0);
	ft_set_main_signals();
	close(ms->pipefd[0]);
	ms->c_stuck_cats = 0;
}

/**
 * @brief  Handles the child process for cleaning stuck "cat" commands.
 * 
 * Reads input from the user in a loop and decrements stuck "cat" counter
 * until the input is terminated or finished.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return void
 */
static void	ft_clean_stuck_cats_child(t_minishell *ms)
{
	char	*input;

	ft_set_heredoc_signals();
	close(ms->pipefd[0]);
	while (ms->c_stuck_cats)
	{
		input = readline("");
		if (!input)
			exit(ft_free_ms(ms, true, true, 0));
		ms->c_stuck_cats--;
		free(input);
	}
	close(ms->pipefd[1]);
	exit(ft_free_ms(ms, true, true, 0));
}

/**
 * @brief  Find and count stuck "cat" commands in the AST.
 * 
 * @param  ms    Pointer to the minishell structure.
 * @param  node  Current node in the AST.
 * @return void
 */
void	ft_find_stuck_cats(t_minishell *ms, t_node *node)
{
	t_node	*current;

	current = node;
	if (!current)
		return ;
	if (current->token->type == TKN_CMD && current->token->value[0] != '\0')
	{
		if (current->cmd_ready[1] == NULL
			&& (!ft_strcmp(current->cmd_ready[0], "cat")
				|| !ft_strcmp(current->cmd_ready[0], "/bin/cat"))
			&& current->prev && current->prev->token->type == TKN_PIPE
			&& (current->prev->left == current
				|| (current->prev->prev
					&& current->prev->prev->token->type == TKN_PIPE
					&& current->prev->right == current)))
			ms->c_stuck_cats++;
	}
	if (!current->left && !current->right)
		return ;
	ft_find_stuck_cats(ms, current->left);
	if (!ms->c_stuck_cats)
		return ;
	ft_find_stuck_cats(ms, current->right);
}
