/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 17:28:29 by bschwell          #+#    #+#             */
/*   Updated: 2025/01/03 10:49:50 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
extern volatile sig_atomic_t g_interrupt;

void	ft_builtin_env(char **args, t_minishell *ms)
{
	char **env;
	
	env = ms->env.envp;
	if (args[1])
	{
		printf("[env error]: doesn't support arguments\n");
		set_exit_code(ms, EX_EXITARGERR);
		return ;
	}
	else
		while (*env != 0)
		{
			printf("%s\n", *env);
			env++;
		}
	set_exit_code(ms, EX_OK);
}
