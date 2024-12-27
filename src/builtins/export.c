/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 11:28:18 by bschwell          #+#    #+#             */
/*   Updated: 2024/12/27 18:07:15 by bschwell         ###   ########.fr       */
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
	printf("%s\n", ms->env.full_path);*/
}

int	ft_set_env(const char *key, const char *value, t_minishell *ms)
{
	int i;
	size_t len;
	char *new_var;

	i = 0;
	len = ft_strlen(key);
	new_var = ft_strjoin(ft_strjoin(key, "="), value);
	if (!key || !value || !ms || !ms->env.envp)
		return (1);
	if (!new_var)
	{
		perror("ft_strjoin");
		return (1);
	}
	while (ms->env.envp[i])
	{
		if (!ft_strncmp(ms->env.envp[i], key, len) && ms->env.envp[i][len] == '=')
		{
			free(ms->env.envp[i]);
			ms->env.envp[i] = new_var;
			return (0);
		}
		i++;
	}
	ms->env.envp[i] = new_var;
	ms->env.envp[i + 1] = NULL;
	return (0);
}

int ft_unset_env(const char *key, t_minishell *ms)
{
	int i;
	int j;
	size_t len;

	len = ft_strlen(key);
	i = 0;
	j = 0;
	if (!key || !ms || !ms->env.envp)
		return (1);
	while (ms->env.envp[i])
	{
		if (!ft_strncmp(ms->env.envp[i], key, len) && ms->env.envp[i][len] == '=')
		{
			free(ms->env.envp[i]);
			j = i;
			while (ms->env.envp[j])
			{
				ms->env.envp[j] = ms->env.envp[j + 1];
				j++;
			}
			return (0);
		}
		i++;
	}
	return (1);
}
