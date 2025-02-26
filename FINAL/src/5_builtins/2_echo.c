/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_echo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 17:18:39 by bschwell          #+#    #+#             */
/*   Updated: 2025/02/26 06:38:42 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void		ft_builtin_echo(char **args, t_minishell *ms);
static int	ft_check_valid_echo_opt(char *opt);

/**
 * @brief 		Check if arg from echo is a valid variation of -n
 * 
 * @param opt	Option that needs to be checked
 * @return int	1: is a valid variation of -n
 * 				0: is not
 */
static int	ft_check_valid_echo_opt(char *opt)
{
	int	i;

	i = 1;
	if (ft_strncmp(opt, "-n", 2) == 0)
	{
		while (opt[i])
		{
			if (opt[i] != 'n')
				return (0);
			i++;
		}
	}
	else
		return (0);
	return (1);
}

/**
 * @brief 	Builtin echo
 * 
 * @param 	args 	What should be written on the command line
 * @param 	ms	 	minishell struct pointer
 */
void	ft_builtin_echo(char **args, t_minishell *ms)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (args[i])
	{
		if (ft_check_valid_echo_opt(args[i]) == 0)
			break ;
		newline = 0;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	ft_set_exit_code(ms, EX_OK);
}
