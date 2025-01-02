/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 09:08:03 by bschwell          #+#    #+#             */
/*   Updated: 2025/01/02 10:12:29 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
extern volatile sig_atomic_t g_interrupt;

static void ft_ensure_trailing_slash(char *path) {
	size_t len = strlen(path);
	if (len > 0 && path[len - 1] != '/')
		ft_strncat(path, "/", PATH_MAX - len - 1);
}

static char *ft_strtok(char *str, const char *delim) {
	static char *last = NULL;
	char *start = NULL;

	if (str != NULL)
		last = str;
	else if (last == NULL)
		return NULL;
	while (*last && strchr(delim, *last) != NULL)
		last++;
	if (*last == '\0') {
		last = NULL;
		return NULL;
	}
	start = last;
	while (*last && strchr(delim, *last) == NULL)
		last++;
	if (*last) {
		*last = '\0';
		last++;
	} 
	else
		last = NULL;
	return start;
}

static void ft_resolve_relative_path(const char *base_path, const char *relative_path, char *resolved_path) {
	char temp_path[PATH_MAX];
	const char *home_dir;
	char result_path[PATH_MAX];
	char *token;

	// Handle paths starting with ~ (home directory)
	if (relative_path[0] == '~') {
		home_dir = getenv("HOME");
		if (!home_dir) {
			printf("Error: HOME environment variable not set.\n");
			exit(EXIT_FAILURE);
		}
		// Replace ~ with home directory
		snprintf(temp_path, PATH_MAX, "%s%s", home_dir, relative_path + 1);
	} else {
		// Copy the base path to a temporary buffer
		ft_strncpy(temp_path, base_path, PATH_MAX - 1);
		temp_path[PATH_MAX - 1] = '\0';

		// Ensure the base path ends with a '/'
		ft_ensure_trailing_slash(temp_path);

		// Append the relative path
		ft_strncat(temp_path, relative_path, PATH_MAX - strlen(temp_path) - 1);
	}

	// Tokenize and resolve relative path elements (., ..)
	result_path[0] = '\0';
	token = ft_strtok(temp_path, "/");
	while (token != NULL) {
		if (ft_strcmp(token, ".") == 0) {
			// Current directory: do nothing
		} else if (ft_strcmp(token, "..") == 0) {
			// Parent directory: remove the last segment from result_path
			char *last_slash = ft_strrchr(result_path, '/');
			if (last_slash != NULL) {
				*last_slash = '\0'; // Remove the last segment
			} else {
				// Edge case: prevent going beyond root
				result_path[1] = '\0';
			}
		} else {
			// Normal directory: append it to the path
			if (ft_strlen(result_path) + ft_strlen(token) + 2 < PATH_MAX) {
				ft_strncat(result_path, "/", PATH_MAX - ft_strlen(result_path) - 1);
				ft_strncat(result_path, token, PATH_MAX - ft_strlen(result_path) - 1);
			}
		}
		token = ft_strtok(NULL, "/");
	}

	// Ensure the resulting path ends with a '/'
	ft_ensure_trailing_slash(result_path);

	// Copy the result to the resolved_path buffer
	ft_strncpy(resolved_path, result_path, PATH_MAX - 1);
	resolved_path[PATH_MAX - 1] = '\0';
}

int		ft_builtin_cd_check(char **args, t_minishell *ms)
{
	char	*curpwd;
	char	resolved_path[PATH_MAX];

	printf("check cd \n");
	curpwd = getcwd(args[1], PATH_MAX);
	if (curpwd == NULL)
		return(ft_builtin_error("getcwd:", errno));
	if (args[1] == NULL)
	{
		if (ft_get_env("HOME", ms) == NULL)
			return (ft_builtin_error("cd: HOME not set", 1));
		ft_strncpy(resolved_path, ft_get_env("HOME", ms), ft_strlen(ft_get_env("HOME", ms)) + 1);
		printf("resolved path %s: %s\n", "HOME", resolved_path);
	}
	else if (args[2] == NULL)
	{
		printf("strcmp: %d\n", ft_strcmp(args[1]);
		if (ft_strcmp(args[1], "-") == 0)
		{
			if (ft_get_env("OLDPWD", ms) == NULL)
				return(ft_builtin_error("cd: OLDPWD not set", 2));
			ft_strncpy(resolved_path, ft_get_env("OLDPWD", ms), ft_strlen(ft_get_env("OLDPWD", ms)) + 1);
			printf("resolved path %s: %s\n", "OLDPWD", resolved_path);
		}
		else
			ft_resolve_relative_path(curpwd, args[1], resolved_path);
		printf("resolved path %s: %s\n", "1 ARG", resolved_path);
	}
	else
		return (printf("cd: too many arguments\n"));
	if (chdir(resolved_path) != 0)
		return (ft_builtin_error("cd error", errno));
	return (0);
}

/**
 * @brief	execute cd command. 3 cases needed:
 * *		 0 arg: check if $HOME exists, change to HOME value
 * *		 1 arg: calculate path and change ENV vars to it
 * 
 * @param args	directory to change to
 * @param ms 	minishell pointer
 */
void	ft_builtin_cd(char **args, t_minishell *ms)
{
	char	curpwd[PATH_MAX];
	char	oldpwd[PATH_MAX];
	char	resolved_path[PATH_MAX];

	printf("execute cd \n");
	if (getcwd(args[1], PATH_MAX) == NULL)
	{
		set_exit_code(ms, errno);
		return ;
	}
	ft_strncpy(curpwd, ft_get_env("PWD", ms), ft_strlen(ft_get_env("PWD", ms)) + 1);
	if (ft_get_env("OLDPWD", ms) != NULL)
		ft_strncpy(oldpwd, ft_get_env("OLDPWD", ms), ft_strlen(ft_get_env("OLDPWD", ms)) + 1);
	if (args[1] == NULL)
		ft_strncpy(resolved_path, ft_get_env("HOME", ms), ft_strlen(ft_get_env("HOME", ms)) + 1);
	else if (args[2] == NULL)
	{
		if (ft_strcmp(args[1], "-") == 0)
			ft_strncpy(resolved_path, ft_get_env("OLDPWD", ms), ft_strlen(ft_get_env("OLDPWD", ms)) + 1);
		else if (ft_strcmp(args[1], "~") == 0)
			ft_strncpy(resolved_path, ft_get_env("HOME", ms), ft_strlen(ft_get_env("HOME", ms)) + 1);
		else
			ft_resolve_relative_path(curpwd, args[1], resolved_path);
	}
	ft_set_env("PWD", resolved_path, ms);
	ft_set_env("OLDPWD", oldpwd, ms);
	set_exit_code(ms, 0);
}

/* HELP FROM BASH CD

cd: cd [-L|[-P [-e]] [-@]] [dir]
    Change the shell working directory.
    
    Change the current directory to DIR.  The default DIR is the value of the
    HOME shell variable. If DIR is "-", it is converted to $OLDPWD.
    
    The variable CDPATH defines the search path for the directory containing
    DIR.  Alternative directory names in CDPATH are separated by a colon (:).
    A null directory name is the same as the current directory.  If DIR begins
    with a slash (/), then CDPATH is not used.
    
    If the directory is not found, and the shell option `cdable_vars' is set,
    the word is assumed to be  a variable name.  If that variable has a value,
    its value is used for DIR.
    
    Options:
      -L	force symbolic links to be followed: resolve symbolic
    		links in DIR after processing instances of `..'
      -P	use the physical directory structure without following
    		symbolic links: resolve symbolic links in DIR before
    		processing instances of `..'
      -e	if the -P option is supplied, and the current working
    		directory cannot be determined successfully, exit with
    		a non-zero status
      -@	on systems that support it, present a file with extended
    		attributes as a directory containing the file attributes
    
    The default is to follow symbolic links, as if `-L' were specified.
    `..' is processed by removing the immediately previous pathname component
    back to a slash or the beginning of DIR.
    
    Exit Status:
    Returns 0 if the directory is changed, and if $PWD is set successfully when
    -P is used; non-zero otherwise.
*/