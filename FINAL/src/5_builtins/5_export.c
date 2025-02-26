/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5_export.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 21:13:47 by bschwell          #+#    #+#             */
/*   Updated: 2025/02/26 06:38:42 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_builtin_export_check(char **args/* , t_minishell *ms */);
void	ft_builtin_export(char **args, t_minishell *ms);
static void	ft_export_1_arg(t_minishell *ms);


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

/**
 * @brief 		Handles export with just 1 argument
 * 
 * @param ms 	minishell pointer
 */
static void	ft_export_1_arg(t_minishell *ms)
{
	char	**dupenv;
	int		count;

	count = -1;
	while (ms->env.envp[++count] != NULL)
		;
	dupenv = malloc(sizeof(char *) * count);
	if (dupenv == NULL)
	{
		perror("malloc error");
		return ;
	}
	ft_dup_envp(ms->env.envp, &dupenv, count);
	ft_sort_envp(dupenv, count);
	ft_output_export_1_arg(dupenv, count);
	ft_free_str_arr(dupenv);
}
