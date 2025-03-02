/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5_export.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 21:13:47 by bschwell          #+#    #+#             */
/*   Updated: 2025/03/02 23:36:28 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int			ft_builtin_export_check(char **args/* , t_minishell *ms */);
void		ft_builtin_export(char **args, t_minishell *ms);
static int	ft_export_1_arg(t_minishell *ms);
static int	ft_valid_export_arg(const char *arg);
static int	ft_process_export(char* str, t_minishell *ms);
static int	ft_export_add_var(char *arg, t_minishell *ms);
static char	**ft_expand_list(char **list, char *new_var);
static int	ft_replace_existing_var(char **list, char *arg);


int	ft_builtin_export_check(char **args/* , t_minishell *ms */)
{
	int		count;

	count = 0;
	while (args[count] != NULL)
		count++;
	return (0);
}

// static char	**ft_get_target_list(char *arg, t_minishell *ms)
// {
// 	if (ft_strchr(arg, '='))
// 		return (ms->env.envp);
// 	return (ms->env.export);
// }

static int	ft_replace_existing_var(char **list, char *arg)
{
	int		i;
	size_t	len;
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
		len = equal_sign - arg;
	else
		len = ft_strlen(arg);
	i = 0;
	while (list[i])
	{
		if (!ft_strncmp(list[i], arg, len)
			&& (list[i][len] == '=' || list[i][len] == '\0'))
		{
			if (!equal_sign && ft_strchr(list[i], '='))
				return (1);
			free(list[i]);
			list[i] = ft_strdup(arg);
			if (!list[i])
				return (-1);
			return (1);
		}
		i++;
	}
	return (0);
}

static char	**ft_expand_list(char **list, char *new_var)
{
	char	**new_list;
	int		count;
	int		i;

	count = 0;
	while (list[count])
		count++;
	new_list = malloc((count + 2) * sizeof(char *));
	if (!new_list)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_list[i] = list[i];
		i++;
	}
	new_list[count] = ft_strdup(new_var);
	if (!new_list[count])
		return (NULL);
	new_list[count + 1] = NULL;
	free(list);
	return (new_list);
}

static int	ft_export_add_var(char *arg, t_minishell *ms)
{
	int		replaced;

	if (ft_strchr(arg, '='))
	{
		replaced = ft_replace_existing_var(ms->env.envp, arg);
		if (replaced == 0)
			ms->env.envp = ft_expand_list(ms->env.envp, arg);
		replaced = ft_replace_existing_var(ms->env.export, arg);
		if (replaced == 0)
			ms->env.export = ft_expand_list(ms->env.export, arg);
	}
	else
	{
		replaced = ft_replace_existing_var(ms->env.export, arg);
		if (replaced == 0)
			ms->env.export = ft_expand_list(ms->env.export, arg);
	}
	return (0);
}

static int	ft_valid_export_arg(const char *arg)
{
	int	i;

	if (!arg || !arg[0])
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

static int	ft_process_export(char *str, t_minishell *ms)
{
	if (!str)
		return (0);
	return (ft_export_add_var(str, ms));
}

void	ft_builtin_export(char **args, t_minishell *ms)
{
	int	i;
	int error;

	i = 1;
	error = 0;
	if (ms->env.export == NULL)
	{
		ms->env.export = ft_duplicate_envp(ms->env.envp);
		if (ms->env.export == NULL)
			ft_perror("malloc", ft_exit_code(1));
	}
	if (args[i] == NULL)
		ft_export_1_arg(ms);
	else
	{
		while (args[i])
		{
			if (ft_valid_export_arg(args[i]) == 0)
			{
				ft_putstr_fd("minishell: export: `", STDERR_FILENO);
				ft_putstr_fd(args[i], STDERR_FILENO);
				ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
				error = 1;
				ft_exit_code(error);
			}
			else
				ft_process_export(args[i], ms);
			i++;
		}
		ft_exit_code(error);
	}
}

/**
 * @brief 		Handles export with just 1 argument
 * 
 * @param ms 	minishell pointer
 */
static int	ft_export_1_arg(t_minishell *ms)
{
	int		count;

	count = -1;
	while (ms->env.envp[++count] != NULL)
		;
	ft_sort_envp(ms->env.export, count);
	ft_output_export_1_arg(ms->env.export, count);
	return (ft_exit_code(0));
}
