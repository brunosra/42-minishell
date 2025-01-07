/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 09:13:42 by bschwell          #+#    #+#             */
/*   Updated: 2025/01/06 12:16:09 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
extern volatile sig_atomic_t g_interrupt;

/**
 * @brief	Check if arguments are valid for unset
 * 
 * @param	arg		string with varname
 * @return	int 
 * *		0: ok
 * *		1: error
 */
static int ft_check_valid_varname(char *arg)
{
	if (!arg[0] || !ft_isalpha(arg[0]) || arg[0] == '_')
		return (1);
	printf("arg[0]: %c\n", arg[0]);
	return (0);
}

/**
 * @brief 	Check if argument is an option (starts with -)
 * 
 * @param 	arg arg to be checked
 * @return 	int
 * *		0: ok
 * *		1: error
 */
static int ft_check_option(char *arg)
{
	if (arg[0] == '-')
	{
		printf("unset: options not implemented\n");
		return (1);
	}
	return (0);
}

/**
 * @brief	Builtin implementation: unset. Unsets variables from envp
 * 
 * @param	args	struct with input from command line
 * @param	ms		pointer to minishell struct
 */
void	ft_builtin_unset(char **args, t_minishell *ms)
{
	size_t	i;

	i = 1;
	if (!args[1])
		return ;
	while (args[i])
	{
		if (i == 1 && ft_check_option(args[i]) != 0)
			break ;
		if (ft_check_valid_varname(args[i]) == 0)
			ft_unset_env(args[i], ms);
		i++;
	}
	set_exit_code(ms, 0);
}

/* 
########################################
BASH HELP FOR UNSET - FOR REFERENCE ONLY
########################################

NAME
    unset - Unset values and attributes of shell variables and functions.

SYNOPSIS
    unset [-f] [-v] [-n] [name ...]

DESCRIPTION
    Unset values and attributes of shell variables and functions.
    
    For each NAME, remove the corresponding variable or function.
    
    Options:
      -f	treat each NAME as a shell function
      -v	treat each NAME as a shell variable
      -n	treat each NAME as a name reference and unset the variable itself
    		rather than the variable it references
    
    Without options, unset first tries to unset a variable, and if that fails,
    tries to unset a function.
    
    Some variables cannot be unset; also see `readonly'.
    
    Exit Status:
    Returns success unless an invalid option is given or a NAME is read-only.

SEE ALSO
    bash(1)

IMPLEMENTATION
    GNU bash, version 5.0.17(1)-release (x86_64-redhat-linux-gnu)
    Copyright (C) 2019 Free Software Foundation, Inc.
    License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
	
############################################
BASH HELP FOR UNSET ----- FOR REFERENCE ONLY
############################################

readonly: readonly [-aAf] [name[=value] ...] or readonly -p
    Mark shell variables as unchangeable.
    
    Mark each NAME as read-only; the values of these NAMEs may not be
    changed by subsequent assignment.  If VALUE is supplied, assign VALUE
    before marking as read-only.
    
    Options:
      -a	refer to indexed array variables
      -A	refer to associative array variables
      -f	refer to shell functions
      -p	display a list of all readonly variables or functions,
    		depending on whether or not the -f option is given
    
    An argument of `--' disables further option processing.
    
    Exit Status:
    Returns success unless an invalid option is given or NAME is invalid.
*/