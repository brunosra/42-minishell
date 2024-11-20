/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hendle_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:50:15 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/11/20 04:35:07 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		ft_revalue_token_variable(t_minishell *ms);
char	*ft_get_env(const char *key, char **envp);
char	**ft_duplicate_envp(char **envp);

char	**ft_duplicate_envp(char **envp)
{
	int		i;
	char	**new_envp;

	i = 0;
	while (envp[i])
		i++;
	new_envp = malloc(sizeof(char *) * (i + 1));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			ft_free_split(new_envp);
			return (NULL);
		}
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

int	ft_revalue_token_variable(t_minishell *ms)
{
	int		i;
	char	*env_value;

	i = -1;
	env_value = NULL;
	if (!ms || !ms->tokens || !ms->env.envp)
		return (1);
	while (ms->tokens[++i].value)
	{
		if (ms->tokens[i].type == TOKEN_VARIABLE)
		{
			env_value = ft_get_env(ms->tokens[i].value + 1, ms->env.envp);
			if (!env_value) // ALterar isto!!!
			{
				ft_putstr_fd("Erro: Variável '", 2); // ALterar isto!!!
				ft_putstr_fd(ms->tokens[i].value, 2); // ALterar isto!!!
				ft_putstr_fd("' não encontrada", 2); // ALterar isto!!!
				return (1);
			}
			free(ms->tokens[i].value);
			ms->tokens[i].value = ft_strdup(env_value);
			if (!ms->tokens[i].value)
			{
				perror("ft_strdup");
				return (1);
			}
		}
	}
	return (0);
}

char	*ft_get_env(const char *key, char **envp)
{
	int		i;
	size_t	len;

	len = ft_strlen(key);
	i = 0;
	if (!key || !envp)
		return (NULL);
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], key, len) && envp[i][len] == '=')
			return (&envp[i][len + 1]);
		i++;
	}
	return (NULL);
}
