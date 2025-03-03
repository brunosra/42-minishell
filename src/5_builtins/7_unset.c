/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   7_unset.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 09:13:42 by bschwell          #+#    #+#             */
/*   Updated: 2025/03/03 19:33:23 by tcosta-f         ###   ########.fr       */
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
	int		err;

	i = 1;
	err = 0;
	if (!args[1])
		return ;
	while (args[i])
	{
		if (i == 1 && ft_check_option(args[i]) != 0)
		{
			err = 1 ;
			ft_exit_code(err);
			break ;
		}
		if (ft_check_valid_varname(args[i]) == 0)
			ft_unset_env(args[i], ms);
		i++;
	}
	ft_exit_code(err);
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
		ft_putstr_fd("minishell: unset: options not implemented\n",
			STDERR_FILENO);
		return (1);
	}
	return (0);
}
