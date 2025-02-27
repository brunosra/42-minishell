/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 04:38:05 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/02/27 18:03:43 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int		ft_perror(char *error, int return_value);
int		ft_putstr_and_return(char *msg, int return_value);
/* void	ft_set_exit_code(t_minishell *ms, int exit_code);
int		ft_exit_code(t_minishell *ms); */
int		ft_exit_code(int newcode);

/**
 * @brief  Print an error message using perror and return a specified value.
 * 
 * @param  error         The error message to display.
 * @param  return_value  The value to return after displaying the error.
 * @return int           The specified return value.
 */
int	ft_perror(char *error, int return_value)
{
	perror(error);
	return (return_value);
}

/**
 * @brief  Print a message to STDERR and return a specified value.
 * 
 * @param  msg           The message to display.
 * @param  return_value  The value to return after displaying the message.
 * @return int           The specified return value.
 */
int	ft_putstr_and_return(char *msg, int return_value)
{
	ft_putstr_fd(msg, STDERR_FILENO);
	return (return_value);
}

/**
 * @brief  Setter for the exit code in the minishell structure.
 * 
 * @param  ms         Pointer to the minishell structure.
 * @param  exit_code  The exit code to set.
 */
/* void	ft_set_exit_code(t_minishell *ms, int exit_code)
{
	ms->exit_code = exit_code;
} */

/**
 * @brief  Getter for the exit code in the minishell structure.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return int The current exit code.
 */
/* int	ft_exit_code(t_minishell *ms)
{
	return (ms->exit_code);
} */

int	ft_exit_code(int newcode)
{
	static int	code = 0;

	if (newcode == -1)
		return (code);
	code = newcode;
	return (code);
}