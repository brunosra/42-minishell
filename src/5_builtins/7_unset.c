/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   7_unset.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 09:13:42 by bschwell          #+#    #+#             */
/*   Updated: 2025/02/27 17:59:15 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void		ft_builtin_unset(char **args, t_minishell *ms);
static int	ft_check_valid_varname(char *arg);
static int	ft_check_option(char *arg);

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
	ft_exit_code(0);
}

/**
 * @brief	Check if arguments are valid for unset
 * 
 * @param	arg		string with varname
 * @return	int 
 * *		0: ok
 * *		1: error
 */
static int	ft_check_valid_varname(char *arg)
{
	if (!arg[0] || !ft_isalpha(arg[0]) || arg[0] == '_')
		return (1);
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
static int	ft_check_option(char *arg)
{
	if (arg[0] == '-')
	{
		printf("unset: options not implemented\n");
		return (1);
	}
	return (0);
}
