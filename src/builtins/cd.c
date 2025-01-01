/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 09:08:03 by bschwell          #+#    #+#             */
/*   Updated: 2024/12/31 14:20:56 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
extern volatile sig_atomic_t g_interrupt;

// TODO: precisa terminar o CD depois que resolver o export
void	ft_builtin_cd(char **args, t_minishell *ms)
{
	int		result;
	char	*oldpwd;
	char	*newpwd;

	// TEST
	/* printf("##############\n");
	ft_print_str_arr(args);
	printf("##############\n");
	printf("arg[1]: %s\n", args[1]); */
	// printf("arg[2]: %s\n", args[2]);
	printf("oldpwd: %s\n", ft_get_env("PWD", ms));
	// ft_print_str_arr(ms->env.envp);
	oldpwd = ft_get_env("PWD", ms);
	printf("oldpwd_var: %s\n", oldpwd);
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
	}
	else
	{
		// more than 2 arguments
		set_exit_code(ms, 1);
		printf("cd: too many arguments\n");
		exit(ms->exit_code);
	}

	result = chdir(newpwd);
	if (result != 0)
		perror("cd error");
	set_exit_code(ms, result);
	printf("newpwd: %s\n", ft_get_env("PWD", ms));
	ms->exit_code = 0;
	// ft_print_str_arr(ms->env.envp);
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