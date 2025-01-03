/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 11:27:32 by bschwell          #+#    #+#             */
/*   Updated: 2025/01/03 11:47:04 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
extern volatile sig_atomic_t g_interrupt;

/**
 * @brief 	Tokenize a string and returns them in order
 * 
 * @param 	str 	String to be tokenized
 * @param 	delim 	Token delimiter
 * @return 	char* 	Pointer to the first char of the first pointer.
 */
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

/**
 * @brief	resolve relative paths for cd
 * 
 * @param	base_path 		the current working directory
 * @param	relative_path 	the path to solve to
 * @param	resolved_path 	pointer to where to store the resolved path
 */

void ft_resolve_relative_path(const char *base_path, const char *relative_path, char *resolved_path) {
    char temp_path[PATH_MAX];
	const char *home_dir;
	char normalized_path[PATH_MAX];
	char *token;
	size_t len;
	char *last_slash;

    if (relative_path[0] == '~') {
        home_dir = getenv("HOME");
        if (relative_path[1] == '\0')
            ft_strncpy(temp_path, home_dir, PATH_MAX - 1);
        else
		{
            ft_strncpy(temp_path, home_dir, PATH_MAX - 1);
            temp_path[PATH_MAX - 1] = '\0';
            ft_strncat(temp_path, relative_path + 1, PATH_MAX - ft_strlen(temp_path) - 1);
        }
    }
	else if (relative_path[0] == '/')
        ft_strncpy(temp_path, relative_path, PATH_MAX - 1);
    else
	{
        ft_strncpy(temp_path, base_path, PATH_MAX - 1);
        temp_path[PATH_MAX - 1] = '\0';
        if (temp_path[strlen(temp_path) - 1] != '/')
            ft_strncat(temp_path, "/", PATH_MAX - ft_strlen(temp_path) - 1);
        ft_strncat(temp_path, relative_path, PATH_MAX - ft_strlen(temp_path) - 1);
    }
    ft_strncpy(normalized_path, "/", PATH_MAX - 1);
    
	// TODO: get all this while into a separate function
	token = ft_strtok(temp_path, "/");
    while (token != NULL) {
        if (ft_strcmp(token, ".") == 0)
			;
		else if (ft_strcmp(token, "..") == 0)
		{
            last_slash = ft_strrchr(normalized_path, '/');
            if (last_slash != NULL && last_slash != normalized_path)
                *last_slash = '\0';
            else
                normalized_path[1] = '\0';
        }
		else
		{
            len = ft_strlen(normalized_path);
            if (len > 1 && normalized_path[len - 1] != '/')
                ft_strncat(normalized_path, "/", PATH_MAX - len - 1);
            ft_strncat(normalized_path, token, PATH_MAX - ft_strlen(normalized_path) - 1);
        }
        token = ft_strtok(NULL, "/");
    }
    ft_strncpy(resolved_path, normalized_path, PATH_MAX - 1);
    resolved_path[PATH_MAX - 1] = '\0';
}

/**
 * @brief 	checks if arguments are valid for cd 
 * 
 * @param 	args 	arguments received in the function
 * @param 	ms 		minishell pointer
 * @return 	int
 * *		0: OK!
 * * 		N: not ok, cd should not be run.
 * TODO: 	remove comments
 */
int		ft_builtin_cd_check(char **args, t_minishell *ms)
{
	char	*curpwd;
	char	resolved_path[PATH_MAX];

	curpwd = ft_get_env("PWD", ms);
	if (curpwd == NULL)
		return (ft_builtin_error("getcwd:", errno));
	if (args[1] == NULL)
	{
		if (ft_get_env("HOME", ms) == NULL)
			return (ft_builtin_error("cd: HOME not set", 1));
		ft_strncpy(resolved_path, ft_get_env("HOME", ms), PATH_MAX - 1);
		// printf("resolved path %s: %s\n", "HOME", resolved_path);
	}
	else if (args[2] == NULL)
	{
		if (ft_strcmp(args[1], "-") == 0)
		{
			if (ft_get_env("OLDPWD", ms) == NULL)
				return(ft_builtin_error("cd: OLDPWD not set", 2));
			ft_strncpy(resolved_path, ft_get_env("OLDPWD", ms), PATH_MAX - 1);
			// printf("resolved path %s: %s\n", "OLDPWD", resolved_path);
		}
		else
		{
			// printf("curpwd : %s\n", curpwd);
			// printf("args[1]: %s\n", args[1]);
			ft_resolve_relative_path(curpwd, args[1], resolved_path);
		}
		// printf("resolved path %s: %s\n", "1 ARG", resolved_path);
	}
	else
		return (printf("cd: too many arguments\n"));
	if (chdir(resolved_path) != 0)
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
	char	resolved_path[PATH_MAX];

	// printf("execute cd \n");
	curpwd = ft_get_env("PWD", ms);
	if (curpwd == NULL)
	{
		set_exit_code(ms, errno);
		return ;
	}
	if (ft_get_env("OLDPWD", ms) != NULL)
		ft_strncpy(oldpwd, ft_get_env("OLDPWD", ms), PATH_MAX - 1);
	if (args[1] == NULL)
		ft_strncpy(resolved_path, ft_get_env("HOME", ms), PATH_MAX - 1);
	else if (args[2] == NULL)
	{
		if (ft_strcmp(args[1], "-") == 0)
			ft_strncpy(resolved_path, ft_get_env("OLDPWD", ms), PATH_MAX - 1);
		else if (ft_strcmp(args[1], "~") == 0)
			ft_strncpy(resolved_path, ft_get_env("HOME", ms), PATH_MAX - 1);
		else
			ft_resolve_relative_path(curpwd, args[1], resolved_path);
	}
	printf("resolved path: %s\n", resolved_path);
	ft_set_env("PWD", resolved_path, ms);
	ft_set_env("OLDPWD", curpwd, ms);
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