/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 11:28:18 by bschwell          #+#    #+#             */
/*   Updated: 2024/12/27 14:45:54 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
extern volatile sig_atomic_t g_interrupt;

void	ft_builtin_export(t_minishell *ms)
{
	/* printf("########## envp: #########\n");
	ft_print_str_arr(ms->env.envp); */
	printf("####### env_paths: #######\n");
	printf("%s\n", ms->env.env_paths);
	/* printf("######### paths: #########\n");
	ft_print_str_arr(ms->env.paths);
	printf("####### full_path: #######\n");
	printf("%s\n", ms->env.full_path);
} */
