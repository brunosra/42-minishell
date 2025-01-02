/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:54:19 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/01/02 12:05:12 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
extern volatile sig_atomic_t g_interrupt;

int		ft_builtin_error(char *msg, int err)
{
	perror(msg);
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
	if (!ft_strcmp(node->token->value, "echo"))
		return (0);
	else if (!ft_strcmp(node->token->value, "exit"))
	 	return (0);
	else if (!ft_strcmp(node->token->value, "pwd"))
	 	return (0);
	else if (!ft_strcmp(node->token->value, "cd"))
		return (ft_builtin_cd_check(node->cmd_ready, ms));
	else if (!ft_strcmp(node->token->value, "env"))
		ft_builtin_env(node->cmd_ready, ms);
	else if (!ft_strcmp(node->token->value, "export"))
		ft_builtin_export(ms);
	return (0);
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
		ft_builtin_echo(node->cmd_ready);
	else if (!ft_strcmp(node->token->value, "exit"))
	 	ft_builtin_exit(node->cmd_ready);
	else if (!ft_strcmp(node->token->value, "env"))
		ft_builtin_env(node->cmd_ready, ms);
	else if (!ft_strcmp(node->token->value, "pwd"))
	 	ft_builtin_pwd(ms);
	else if (!ft_strcmp(node->token->value, "cd"))
		ft_builtin_cd(node->cmd_ready, ms);
	else if (!ft_strcmp(node->token->value, "export"))
			ft_builtin_export(ms);
	return (exit_code(ms));
}
