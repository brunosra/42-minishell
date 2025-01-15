/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 02:09:28 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/01/15 04:48:29 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
extern volatile sig_atomic_t g_interrupt;

int ft_perror(char *error, int return_value);
int ft_putstr_and_return(char *msg, int return_value);
void	ft_set_exit_code(t_minishell *ms, int exit_code);
int		ft_exit_code(t_minishell *ms);
char *ft_str_join_all(int argcount, ...);

/**
 * @brief  Print an error message using perror and return a specified value.
 * 
 * @param  error         The error message to display.
 * @param  return_value  The value to return after displaying the error.
 * @return int           The specified return value.
 */
int ft_perror(char *error, int return_value)
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
int ft_putstr_and_return(char *msg, int return_value)
{
	ft_putstr_fd(msg, STDERR_FILENO);
	return (return_value);
}

/**
 * @brief Getter and Setter for Exit Codes
 * 
 * @param ms 
 * @param exit_code 
 */
void	ft_set_exit_code(t_minishell *ms, int exit_code)
{
	ms->exit_code = exit_code;
}

/**
 * @brief  Get the current exit code from the minishell structure.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return int The current exit code.
 */
int	ft_exit_code(t_minishell *ms)
{
	return (ms->exit_code);
}

/**
 * @brief  Concatenate multiple strings into a single string.
 * 
 * @param  argcount  The number of strings to concatenate.
 * @param  ...       Variadic arguments representing the strings to join.
 * @return char*     Pointer to the newly allocated concatenated string.
 **                  NULL if memory allocation fails.
 */
char *ft_str_join_all(int argcount, ...)
{
	va_list	ap;
	char *tmp;
	char *to_free;
	char *curr_str;
	int curr_arg;

	curr_arg = 0;
	tmp = malloc(1);
	tmp[0] = 0;
	va_start(ap, argcount);
	while (curr_arg++ < argcount)
	{
		curr_str = va_arg(ap, char*);
		to_free = tmp;
		tmp = ft_strjoin(tmp, curr_str);
		free(to_free);
	}
	va_end(ap);
	return (tmp);
}