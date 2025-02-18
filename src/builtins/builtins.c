/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:54:19 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/02/18 18:22:59 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
extern volatile int	g_interrupt;

int	ft_builtin_error(char *msg, int err);
int	ft_exec_builtins_check(t_node *node, t_minishell *ms);
int	ft_exec_builtins(t_node *node, t_minishell *ms);

/**
 * @brief  Print an error message using perror and return a specified error code.
 * 
 * @param  msg  The error message to display.
 * @param  err  The error code to return.
 * @return int  The specified error code.
 */
int	ft_builtin_error(char *msg, int err) // temos mais duas funcoes que fazem o mesmo! UNIFORMIZAR
{
	ft_putstr_fd(msg, STDERR_FILENO);
	return (err);
}

/**
 * @brief 	Check if builtin executed correctly
 * 
 * @param 	node	tree node
 * @param 	ms 		minishell pointer
 * @return 	int 
 ** 	  	0 in case of success
 **			N (any number) in case of error
 */
int	ft_exec_builtins_check(t_node *node, t_minishell *ms)
{
	if (!ft_strcmp(node->token->value, "echo") ||
		!ft_strcmp(node->token->value, "exit") ||
		!ft_strcmp(node->token->value, "pwd") ||
		!ft_strcmp(node->token->value, "env"))
		return (0);
	else if (!ft_strcmp(node->token->value, "cd"))
		return (ft_builtin_cd_check(node->cmd_ready, ms));
	else if (!ft_strcmp(node->token->value, "export"))
		ft_builtin_export_check(node->cmd_ready, ms);
	// else if (!ft_strcmp(node->token->value, "unset"))
	// 	return (ft_builtin_unset_check(node->cmd_ready));
	return (EX_OK);
}

/**
 * @brief 	Executes builtin if check allows
 * 
 * @param 	node 	tree node
 * @param 	ms 		minishell ptr
 * @return 	int 	exit code that was in the ms struct
 */
int	ft_exec_builtins(t_node *node, t_minishell *ms)
{
	if (!ft_strcmp(node->token->value, "echo"))
		ft_builtin_echo(node->cmd_ready, ms);
	else if (!ft_strcmp(node->token->value, "exit"))
	 	ft_builtin_exit(node->cmd_ready);
	else if (!ft_strcmp(node->token->value, "env"))
		ft_builtin_env(node->cmd_ready, ms);
	else if (!ft_strcmp(node->token->value, "pwd"))
	 	ft_builtin_pwd(ms);
	else if (!ft_strcmp(node->token->value, "cd"))
		ft_builtin_cd(node->cmd_ready, ms);
	else if (!ft_strcmp(node->token->value, "export"))
		ft_builtin_export(node->cmd_ready, ms);
	else if (!ft_strcmp(node->token->value, "unset"))
		ft_builtin_unset(node->cmd_ready, ms);
	return (ft_exit_code(ms));
}
