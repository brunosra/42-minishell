/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 20:47:54 by bschwell          #+#    #+#             */
/*   Updated: 2025/02/18 21:09:06 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern volatile int	g_interrupt;
static int			ft_value_is_numeric(char *str);
static long long	ft_atoll(char *str, int i, long long res);
int					ft_builtin_exit(char **args);
static void			ft_putstr_and_exit(char *str);

/**
 * @brief	Check if a string contains numeric characters.
 * 
 * @param	str	String to check for numeric characters.
 * @return	int
 * 			1 if the string contains numeric characters, 0 otherwise.
 */
static int	ft_value_is_numeric(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isdigit(str[i]))
			return (1);
		i++;
	}
	return (0);
}

static void	ft_putstr_and_exit(char *str)
{
	ft_putstr_fd("exit\nminishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	exit(2);
}

/**
 * @brief	Convert a string to a long long integer with error handling.
 * 
 * @param	str	String to be converted.
 * @param	i	Initial offset in the string for conversion.
 * @param	res	Initial value to use for the result (typically 0).
 * @return	long long
 * 			The converted value as a signed long long integer.
 * 
 * @details
 * - Handles leading whitespace and optional '+' or '-' signs.
 * - Detects overflow or invalid characters in the input string.
 * - Prints an error message and exits with code 2 if
 * 	 the string is not numeric or overflows.
 */
static long long	ft_atoll(char *str, int i, long long res)
{
	int	sig;

	sig = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			sig = -1;
	if (str[i] >= '0' && str[i] <= '9')
	{
		while (str[i] >= '0' && str[i] <= '9')
		{
			if (res > (LLONG_MAX / 10) || (res == LLONG_MAX / 10
					&& (str[i] - '0') > (LLONG_MAX % 10)))
				ft_putstr_and_exit(str);
			res = res * 10 + (str[i] - '0');
			i++;
		}
		if (str[i] != '\0')
			ft_putstr_and_exit(str);
	}
	else
		ft_putstr_and_exit(str);
	return (res * sig);
}

static void	ft_putstr_and_not_exit(char *str)
{
	ft_putstr_fd(MSG_ERR_EXIT, STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
}

/**
 * @brief	Implement the `exit` builtin command.
 * 
 * @param	args	Array of arguments passed to the `exit` command.
 * @return	int
 * 			Exit code to be used when terminating the shell.
 * 
 * @details
 * - No arguments: Returns 0, indicating a successful exit.
 * - Single argument: Converts the argument to a numeric exit code modulo 256.
 * - Multiple arguments: Prints an error message for too
 * 						 many arguments and returns 1.
 * - Non-numeric argument: Prints an error message and returns 2.
 */
int	ft_builtin_exit(char **args)
{
	int	mod;
	int	i;

	mod = 0;
	i = 0;
	while (args[i])
		i++;
	if (i == 1)
		return (0);
	else if (i > 2 && ft_atoll(args[1], 0, 0))
	{
		ft_putstr_fd(MSG_ERR_EXIT" too many arguments\n", STDERR_FILENO);
		return (1);
	}
	else if (ft_value_is_numeric(args[1]))
	{
		mod = ft_atoll(args[1], 0, 0) % 256;
		return (mod);
	}
	else if (!ft_value_is_numeric(args[1]))
	{
		ft_putstr_and_not_exit(args[1]);
		return (2);
	}
	return (0);
}
