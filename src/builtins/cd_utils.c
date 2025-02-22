/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 11:27:32 by bschwell          #+#    #+#             */
/*   Updated: 2025/02/22 18:10:40 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern volatile int	g_interrupt;

static int	ft_cd_set_resolved_path_check(char **args, char *curpwd,
				char *res_p, t_minishell *ms)
{
	if (args[1] == NULL)
	{
		if (ft_get_env("HOME", ms) == NULL)
			return (ft_builtin_error("cd: HOME not set", 1));
		ft_strncpy(res_p, ft_get_env("HOME", ms), PATH_MAX - 1);
	}
	else if (args[2] == NULL)
	{
		if (!ft_strcmp(args[1], "-"))
		{
			if (ft_get_env("OLDPWD", ms) == NULL)
				return (ft_builtin_error("cd: OLDPWD not set", 2));
			ft_strncpy(res_p, ft_get_env("OLDPWD", ms), PATH_MAX - 1);
		}
		else
			ft_resolve_rel_p(curpwd, args[1], res_p);
	}
	else
		return (ft_builtin_error("minishell: cd: too many arguments", 1));
	return (0);
}

int	ft_builtin_cd_check(char **args, t_minishell *ms)
{
	char	*curpwd;
	char	res_p[PATH_MAX];
	int		ret;

	curpwd = ft_get_env("PWD", ms);
	if (curpwd == NULL)
		return (ft_builtin_error("getcwd:", errno));
	ret = ft_cd_set_resolved_path_check(args, curpwd, res_p, ms);
	if (ret != 0)
		return (ret);
	if (chdir(res_p) != 0)
	{
		printf("Minishell: cd: %s: No such file or directory\n", args[1]);
		return (errno);
	}
	return (0);
}
