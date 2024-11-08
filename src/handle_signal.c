/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:57:36 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/11/08 02:58:23 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void ft_signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		rl_replace_line("", 0); //limpa a linha
		rl_on_new_line();
		rl_redisplay; //exibe um novo prompt;
	}
	if (signum == SIGQUIT)
	{
		// TODO
	}
}

