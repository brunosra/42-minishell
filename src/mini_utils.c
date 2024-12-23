/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 02:09:28 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/12/23 19:42:45 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
extern volatile sig_atomic_t g_interrupt;

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

void	set_exit_code(t_minishell *ms, int exit_code)
{
	ms->exit_code = exit_code;
}

int		exit_code(t_minishell *ms)
{
	return (ms->exit_code);
}

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