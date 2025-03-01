/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5_export.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 21:13:47 by bschwell          #+#    #+#             */
/*   Updated: 2025/03/01 18:52:45 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int			ft_builtin_export_check(char **args/* , t_minishell *ms */);
void		ft_builtin_export(char **args, t_minishell *ms);
static int	ft_export_1_arg(t_minishell *ms);

// TODO: implement
int	ft_builtin_export_check(char **args/* , t_minishell *ms */)
{
	int		count;

	count = 0;
	while (args[count] != NULL)
		count++;
	// printf("export check: %s\n", ms->env.envp[0]);
	return (0);
}

int	ft_valid_export_arg(const char *arg)
{
	int	i;

	if (!arg)
		return (0);
	if (!arg[0])
		return (0);
	if (!((arg[0] >= 'A' && arg[0] <= 'Z')
		|| (arg[0] >= 'a' && arg[0] <= 'z')
		|| (arg[0] == '_')))
		return (0);
	i = 1;
	while (arg[i] && arg[i] != '=')
	{
		if (!((arg[i] >= 'A' && arg[i] <= 'Z')
			|| (arg[i] >= 'a' && arg[i] <= 'z')
			|| (arg[i] >= '0' && arg[i] <= '9')
			|| (arg[i] == '_')))
			return (0);
		i++;
	}
	return (1);
}

static int	ft_process_export(char* str)
{
	if (str)
		return (0);
	return (1);
}

void	ft_builtin_export(char **args, t_minishell *ms)
{
	int	i;

	i = 0;
	if (args[1] == NULL)
		ft_export_1_arg(ms);
	else
	{
		while (args[i++])
		{
			if (ft_valid_export_arg(args[i]) == 0)
				ft_exit_code(1);
			else
				ft_process_export(args[i]);
		}
	}
	
}

/**
 * @brief 		Handles export with just 1 argument
 * 
 * @param ms 	minishell pointer
 */
static int	ft_export_1_arg(t_minishell *ms)
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
		return (ft_exit_code(1));
	}
	ft_dup_envp(ms->env.envp, &dupenv, count);
	ft_sort_envp(dupenv, count);
	ft_output_export_1_arg(dupenv, count);
	ft_free_str_arr(dupenv);
	return (ft_exit_code(0));
}
