/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_execute_command.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 02:43:45 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/02/27 00:59:56 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int			ft_execute_command(t_node *node, t_minishell *ms);
static void	ft_handle_cmd_exit_status(t_node *node, t_minishell *ms);
static void	ft_execute_child_process(t_node *node, t_minishell *ms);
static int	ft_execute_external(t_node *node, t_minishell *ms);
static void	ft_execute_builtin(t_node *node, t_minishell *ms);

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
	if (ft_exit_code(ms) != 0 && node->prev
		&& node->prev->token->type == TKN_IN_RD)
		ft_remove_created_files(node->prev);
	if (node->token->type == TKN_BLTIN && !ft_strcmp(node->cmd_ready[0], "exit")
		&& ft_exit_code(ms) != 1)
	{
		ft_free_tokens(ms->tokens);
		ft_free_ast(ms->ast_root);
		free(ms->input);
		exit(ft_exit_code(ms));
	}
	return (ft_exit_code(ms));
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
		else if (ft_exit_code(ms) == 0 && node->token->type == TKN_BLTIN
			&& ft_strcmp(node->token->value, "echo"))
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
	if (node->cmd_ready[1] == NULL && !ft_strcmp(node->cmd_ready[0], "cat")
		&& node->prev && node->prev->token->type == TKN_PIPE
		&& (node->prev->left == node
			|| (node->prev->prev && node->prev->prev->token->type == TKN_PIPE
				&& node->prev->right == node)))
		exit(13);
	execve(ms->env.full_path, node->cmd_ready, ms->env.envp);
	perror("execve");
	exit(127);
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
