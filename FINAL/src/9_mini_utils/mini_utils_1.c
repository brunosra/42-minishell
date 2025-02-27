/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 04:38:05 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/02/27 03:30:15 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int		ft_perror(char *error, int return_value);
int		ft_putstr_and_return(char *msg, int return_value);
void	ft_set_exit_code(t_minishell *ms, int exit_code);
int		ft_exit_code(t_minishell *ms);

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
void	ft_set_exit_code(t_minishell *ms, int exit_code)
{
	ms->exit_code = exit_code;
}

/**
 * @brief  Getter for the exit code in the minishell structure.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return int The current exit code.
 */
int	ft_exit_code(t_minishell *ms)
{
	return (ms->exit_code);
}
