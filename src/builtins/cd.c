/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 09:08:03 by bschwell          #+#    #+#             */
/*   Updated: 2025/01/01 18:52:34 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
extern volatile sig_atomic_t g_interrupt;

static void ft_ensure_trailing_slash(char *path) {
    size_t len = strlen(path);
    if (len > 0 && path[len - 1] != '/') {
        strncat(path, "/", PATH_MAX - len - 1);
    }
}

static char *ft_strtok(char *str, const char *delim) {
    static char *last = NULL; // Holds the remainder of the string between calls
    char *start = NULL;       // Pointer to the beginning of the next token

    // If a new string is provided, start from it; otherwise, continue from the last tokenized position
    if (str != NULL) {
        last = str;
    } else if (last == NULL) {
        return NULL; // No more tokens to process
    }

    // Skip leading delimiters
    while (*last && strchr(delim, *last) != NULL) {
        last++;
    }

    // If we've reached the end of the string, return NULL
    if (*last == '\0') {
        last = NULL;
        return NULL;
    }

    // Mark the start of the token
    start = last;

    // Find the end of the token (next delimiter or end of string)
    while (*last && strchr(delim, *last) == NULL) {
        last++;
    }

    // If we found a delimiter, replace it with '\0' and update the position
    if (*last) {
        *last = '\0';
        last++;
    } else {
        // If we've reached the end of the string, set last to NULL
        last = NULL;
    }

    return start;
}

static void resolve_relative_path(const char *base_path, const char *relative_path, char *resolved_path) {
    char temp_path[PATH_MAX];
	const char *home_dir;
	char result_path[PATH_MAX];
	char *token;

    // Handle paths starting with ~ (home directory)
    if (relative_path[0] == '~') {
        home_dir = getenv("HOME");
        if (!home_dir) {
            printf(stderr, "Error: HOME environment variable not set.\n");
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
    result_path = "";
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

// TODO: precisa terminar o CD depois que resolver o export
int		ft_builtin_cd_check(char **args, t_minishell *ms)
{
	char	*newpwd;

	if (args[1] == NULL)
	{
		// no argument
		newpwd = ft_get_env("HOME", ms);
		if (newpwd == NULL)
			return(printf("cd: HOME not set"));
		else
			return (0);
	}
	else if (args[2] == NULL)
	{
		// only 1 argument;
		newpwd = args[1];
		return(0);
	}
	else
	{
		// more than 2 arguments
		return (printf("cd: too many arguments\n"));
	}
	if (chdir(newpwd) != 0)
	{
		perror("chdir error");
		return (errno);
	}
	return (0);
}

void	ft_builtin_cd(char **args, t_minishell *ms)
{
	int		result;
	char	*oldpwd;
	char	*newpwd;

	// ft_print_str_arr(ms->env.envp);
	oldpwd = ft_get_env("PWD", ms);
	if (args[1] == NULL)
	{
		// no argument
		// TODO: check if it works in bash if HOME env doesnt exist.
		newpwd = ft_get_env("HOME", ms);
		if (newpwd == NULL)
			printf("cd: HOME not set");
		else
		{
			ft_set_env("PWD", newpwd, ms);
			ft_set_env("OLDPWD", oldpwd, ms);
		}
	}
	else if (args[2] == NULL)
	{
		// only 1 argument;
		newpwd = args[1];
		if (ft_strcmp(newpwd, "-") == 0)
		{
			printf("%s\n", ft_get_env("OLDPWD", ms));
			newpwd = ft_get_env("OLDPWD", ms);
		}
		else if (ft_strcmp(newpwd, "~") == 0)
			newpwd = ft_get_env("HOME", ms);
	}
	else
	{
		// more than 2 arguments
		set_exit_code(ms, 1);
		printf("cd: too many arguments\n");
	}
	result = chdir(newpwd);
	if (result != 0)
		perror("cd error");
	ft_set_env("PWD", newpwd, ms);
	ft_set_env("OLDPWD", oldpwd, ms);
	set_exit_code(ms, result);
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