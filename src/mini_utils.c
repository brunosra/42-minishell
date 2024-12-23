/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 02:09:28 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/12/23 16:31:01 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int ft_perror(char *error, int return_value);
int ft_putstr_and_return(char *msg, int return_value);

int ft_perror(char *error, int return_value)
{
	perror(error);
	return (return_value);
}

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

void	set_exit_code(t_minishell ms, int exit_code)
{
	ms->exit_code = exit_code;
}

int		exit_code(t_minishell ms)
{
	return (ms->exit_code);
}