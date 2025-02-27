/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_echo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 17:18:39 by bschwell          #+#    #+#             */
/*   Updated: 2025/02/27 18:42:02 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void		ft_builtin_echo(char **args);
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
void	ft_builtin_echo(char **args)
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
	ft_exit_code(EX_OK);
}
