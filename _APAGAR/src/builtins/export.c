/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 21:13:47 by bschwell          #+#    #+#             */
/*   Updated: 2025/02/25 16:37:55 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// TODO: implement
int	ft_builtin_export_check(char **args/* , t_minishell *ms */)
{
	int		count;

	count = 0;
	while (args[count] != NULL)
	{
		count++;
	}
	// printf("export check: %s\n", ms->env.envp[0]);
	return (0);
}

void	ft_builtin_export(char **args, t_minishell *ms)
{
	if (args[1] == NULL)
		ft_export_1_arg(ms);
}
