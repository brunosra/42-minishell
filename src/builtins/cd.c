/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 11:27:32 by bschwell          #+#    #+#             */
/*   Updated: 2025/02/18 20:10:50 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern volatile int	g_interrupt;
static char	*ft_strtok(char *str, const char *delim);
void		ft_resolve_rel_p(const char *base_p,
				const char *rel_p, char *res_p);
int			ft_builtin_cd_check(char **args, t_minishell *ms);
void		ft_builtin_cd(char **args, t_minishell *ms);

/**
 * @brief 	Tokenize a string and returns them in order
 * 
 * @param 	str 	String to be tokenized
 * @param 	delim 	Token delimiter
 * @return 	char* 	Pointer to the first char of the first pointer.
 */
static char	*ft_strtok(char *str, const char *delim)
{
	static char	*last = NULL;
	char		*start;

	if (str != NULL)
		last = str;
	else if (last == NULL)
		return (NULL);
	while (*last && strchr(delim, *last) != NULL)
		last++;
	if (*last == '\0')
	{
		last = NULL;
		return (NULL);
	}
	start = last;
	while (*last && strchr(delim, *last) == NULL)
		last++;
	if (*last)
	{
		*last = '\0';
		last++;
	}
	else
		last = NULL;
	return (start);
}

/**
 * @brief 			Normatizes path
 * 
 * @param tmp_p		temporary path from main function
 * @param norm_p 	normatized path pointer from main function
 * @return char* 	pointer to the normatized path
 */
static char	*ft_norm_pth(char *tmp_p, char *norm_p)
{
	char		*token;

	ft_strncpy(norm_p, "/", PATH_MAX - 1);
	token = ft_strtok(tmp_p, "/");
	while (token != NULL)
	{
		if (ft_strcmp(token, ".") == 0)
			;
		else if (ft_strcmp(token, "..") == 0)
		{
			if (ft_strrchr(norm_p, '/') != NULL
				&& ft_strrchr(norm_p, '/') != norm_p)
				*(ft_strrchr(norm_p, '/')) = '\0';
			else
				norm_p[1] = '\0';
		}
		else
		{
			if (ft_strlen(norm_p) > 1 && norm_p[ft_strlen(norm_p) - 1] != '/')
				ft_strncat(norm_p, "/", PATH_MAX - ft_strlen(norm_p) - 1);
			ft_strncat(norm_p, token, PATH_MAX - ft_strlen(norm_p) - 1);
		}
		token = ft_strtok(NULL, "/");
	}
	return (norm_p);
}

/**
 * @brief	resolve relative paths for cd
 * 
 * @param	base_p 		the current working directory
 * @param	rel_p 		relative path to solve to
 * @param	res_p 		pointer to where to store the resolved path
 */
void	ft_resolve_rel_p(const char *base_p,
		const char *rel_p, char *res_p)
{
	char		tmp_p[PATH_MAX];
	char		norm_pth[PATH_MAX];
	
	if (rel_p[0] == '~')
	{
		if (rel_p[1] == '\0')
			ft_strncpy(tmp_p, getenv("HOME"), PATH_MAX - 1);
		else
		{
			ft_strncpy(tmp_p, getenv("HOME"), PATH_MAX - 1);
			tmp_p[PATH_MAX - 1] = '\0';
			ft_strncat(tmp_p, rel_p + 1, PATH_MAX - ft_strlen(tmp_p) - 1);
		}
	}
	else if (rel_p[0] == '/')
		ft_strncpy(tmp_p, rel_p, PATH_MAX - 1);
	else
	{
		ft_strncpy(tmp_p, base_p, PATH_MAX - 1);
		tmp_p[PATH_MAX - 1] = '\0';
		if (tmp_p[strlen(tmp_p) - 1] != '/')
			ft_strncat(tmp_p, "/", PATH_MAX - ft_strlen(tmp_p) - 1);
		ft_strncat(tmp_p, rel_p, PATH_MAX - ft_strlen(tmp_p) - 1);
	}
	ft_strncpy(res_p, ft_norm_pth(tmp_p, norm_pth), PATH_MAX - 1);
	res_p[PATH_MAX - 1] = '\0';
}

/* void	ft_resolve_rel_p(const char *base_p,
		const char *rel_p, char *res_p)
{
	char		tmp_p[PATH_MAX];
	const char	*home_dir;
	char		norm_pth[PATH_MAX];
	char		*token;
	size_t		len;
	char		*last_slash;

	if (rel_p[0] == '~')
	{
		home_dir = getenv("HOME");
		if (rel_p[1] == '\0')
			ft_strncpy(tmp_p, home_dir, PATH_MAX - 1);
		else
		{
			ft_strncpy(tmp_p, home_dir, PATH_MAX - 1);
			tmp_p[PATH_MAX - 1] = '\0';
			ft_strncat(tmp_p, rel_p + 1, PATH_MAX - ft_strlen(tmp_p) - 1);
		}
	}
	else if (rel_p[0] == '/')
		ft_strncpy(tmp_p, rel_p, PATH_MAX - 1);
	else
	{
		ft_strncpy(tmp_p, base_p, PATH_MAX - 1);
		tmp_p[PATH_MAX - 1] = '\0';
		if (tmp_p[strlen(tmp_p) - 1] != '/')
			ft_strncat(tmp_p, "/", PATH_MAX - ft_strlen(tmp_p) - 1);
		ft_strncat(tmp_p, rel_p, PATH_MAX - ft_strlen(tmp_p) - 1);
	}
	ft_strncpy(norm_pth, "/", PATH_MAX - 1);

	// TODO: get all this while into a separate function
	token = ft_strtok(tmp_p, "/");
	while (token != NULL)
	{
		if (ft_strcmp(token, ".") == 0)
			;
		else if (ft_strcmp(token, "..") == 0)
		{
			last_slash = ft_strrchr(norm_pth, '/');
			if (last_slash != NULL && last_slash != norm_pth)
				*last_slash = '\0';
			else
				norm_pth[1] = '\0';
		}
		else
		{
			len = ft_strlen(norm_pth);
			if (len > 1 && norm_pth[len - 1] != '/')
				ft_strncat(norm_pth, "/", PATH_MAX - len - 1);
			ft_strncat(norm_pth, token, PATH_MAX - ft_strlen(norm_pth) - 1);
		}
		token = ft_strtok(NULL, "/");
	}
	ft_strncpy(res_p, norm_pth, PATH_MAX - 1);
	res_p[PATH_MAX - 1] = '\0';
} */

/**
 * @brief 	checks if arguments are valid for cd 
 * 
 * @param 	args 	arguments received in the function
 * @param 	ms 		minishell pointer
 * @return 	int
 * *		0: OK!
 * * 		N: not ok, cd should not be run.
 */
int	ft_builtin_cd_check(char **args, t_minishell *ms)
{
	char	*curpwd;
	char	res_p[PATH_MAX];

	curpwd = ft_get_env("PWD", ms);
	if (curpwd == NULL)
		return (ft_builtin_error("getcwd:", errno));
	if (args[1] == NULL)
	{
		if (ft_get_env("HOME", ms) == NULL)
			return (ft_builtin_error("cd: HOME not set", 1));
		ft_strncpy(res_p, ft_get_env("HOME", ms), PATH_MAX - 1);
	}
	else if (args[2] == NULL)
	{
		if (ft_strcmp(args[1], "-") == 0)
		{
			if (ft_get_env("OLDPWD", ms) == NULL)
				return (ft_builtin_error("cd: OLDPWD not set", 2));
			ft_strncpy(res_p, ft_get_env("OLDPWD", ms), PATH_MAX - 1);
		}
		else
			ft_resolve_rel_p(curpwd, args[1], res_p);
	}
	else
		return (ft_builtin_error("minishell: cd: too many arguments\n", 1));
	if (chdir(res_p) != 0)
		return (ft_builtin_error("cd error", errno));
	return (0);
}

/**
 * @brief	execute cd command. 3 cases needed:
 * *		0 arg: check if $HOME exists, change to HOME value
 * *		1 arg: calculate path and change ENV vars to it
 * 
 * @param 	args	directory to change to
 * @param 	ms 		minishell pointer
 */
void	ft_builtin_cd(char **args, t_minishell *ms)
{
	char	*curpwd;
	char	oldpwd[PATH_MAX];
	char	res_p[PATH_MAX];

	curpwd = ft_get_env("PWD", ms);
	if (curpwd == NULL)
	{
		ft_set_exit_code(ms, errno);
		return ;
	}
	if (ft_get_env("OLDPWD", ms) != NULL)
		ft_strncpy(oldpwd, ft_get_env("OLDPWD", ms), PATH_MAX - 1);
	if (args[1] == NULL)
		ft_strncpy(res_p, ft_get_env("HOME", ms), PATH_MAX - 1);
	else if (args[2] == NULL)
	{
		if (ft_strcmp(args[1], "-") == 0)
			ft_strncpy(res_p, ft_get_env("OLDPWD", ms), PATH_MAX - 1);
		else if (ft_strcmp(args[1], "~") == 0)
			ft_strncpy(res_p, ft_get_env("HOME", ms), PATH_MAX - 1);
		else
			ft_resolve_rel_p(curpwd, args[1], res_p);
	}
	ft_set_env("OLDPWD", curpwd, ms);
	ft_set_env("PWD", res_p, ms);
	ft_strlcpy(ms->currpath, res_p, ft_strlen(res_p) + 1);
	chdir(ms->currpath);
	ft_set_exit_code(ms, 0);
}
