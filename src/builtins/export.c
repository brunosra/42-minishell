/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 21:13:47 by bschwell          #+#    #+#             */
/*   Updated: 2025/02/23 09:12:51 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern volatile int	g_interrupt;

// TODO: implement
int	ft_builtin_export_check(char **args, t_minishell *ms)
{
	int		count;

	count = 0;
	while (args[count] != NULL)
	{
		count++;
	}
	printf("export check: %s\n", ms->env.envp[0]);
	return (0);
}

void	ft_builtin_export(char **args, t_minishell *ms)
{
	if (args[1] == NULL)
		ft_export_1_arg(ms);
}
