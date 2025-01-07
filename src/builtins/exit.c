/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 17:24:37 by bschwell          #+#    #+#             */
/*   Updated: 2025/01/01 12:16:08 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
extern volatile sig_atomic_t g_interrupt;

static int ft_value_is_numeric(char *str)
{
	int i;

	i = 0;	
	while (str[i])
	{
		if (ft_isdigit(str[i]))
			return (1);
		i++;
	}
	return (0);
}

/**
 * @brief	Str to Long Long conversion
 * 
 * @param 	str	String to be converted
 * @param 	i	Initial Offset in the string to convert 
 * @param 	res	Initial value to be added to object
 * @return long long number
 */
static long long ft_atoll(char *str, int i, long long res)
{
    int sig;

    sig = 1;
    while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
        i++;
    if (str[i] == '+' || str[i] == '-')
    {
        if (str[i] == '-')
            sig = -1;
        i++;
    }
    if (str[i] >= '0' && str[i] <= '9')
	{
		while (str[i] >= '0' && str[i] <= '9')
    	{
        	if (res > (LLONG_MAX / 10) || (res == LLONG_MAX / 10 && (str[i] - '0') > (LLONG_MAX % 10)))
        	{
				ft_putstr_fd("exit\nminishell: exit: ", STDERR_FILENO);
				ft_putstr_fd(str, STDERR_FILENO);
				ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
				exit(2);
			}
        	res = res * 10 + (str[i] - '0');
        	i++;
    	}
		if (str[i] != '\0')
		{
			ft_putstr_fd("exit\nminishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(str, STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			exit(2);
		}
	}
	else
	{
		ft_putstr_fd("exit\nminishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		exit(2);
	}
	return (res * sig);
}

int	ft_builtin_exit(char **args)
{
	int mod;
	int i;

	mod = 0;
	i = 0;
	while (args[i])	
		i++;
	if (i == 1)
		return (0);
		// set_exit_code(ms, 0);
	else if (i > 2 && ft_atoll(args[1], 0, 0))
	{
		ft_putstr_fd("exit\nminishell: exit: too many arguments\n", STDERR_FILENO);
		// exit(1);
		return (1);
	}
	else if (ft_value_is_numeric(args[1]))
	{
		mod = ft_atoll(args[1], 0, 0) % 256;
		// set_exit_code(ms, mod);
		// exit(exit_code(ms));
		return (mod);
	}
	else if (!ft_value_is_numeric(args[1]))
	{
		ft_putstr_fd("exit\nminishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		// exit(2);
		return (2);
	}
	else
		return (0);
		// set_exit_code(ms, 0);
	return (0);
	// exit(exit_code(ms));
}
