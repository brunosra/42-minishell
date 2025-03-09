/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 08:24:42 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/03/09 08:37:26 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_signal_pipe_handler(int sig);
void	ft_set_pipe_signals(void);

/**
 * @brief  Handles signals for pipe child processes.
 * 
 * @param  sig  The received signal.
 * 
 * If the process receives SIGTERM, it frees the minishell structure 
 * and exits safely to prevent zombie processes.
 */
void	ft_signal_pipe_handler(int sig)
{
	t_minishell	*ms;

	if (sig == SIGTERM)
	{
		ms = ft_ms_struct(NULL, 1);
		ft_free_ms(ms, true, true, -1);
		exit(-1);
	}
}

/**
 * @brief  Configures signal handling for child processes in a pipe.
 * 
 * This function ensures that:
 * - SIGTERM is handled to clean up processes properly.
 * - SIGQUIT (Ctrl + \) is ignored.
 * - SIGINT (Ctrl + C) is ignored to prevent premature termination.
 */
void	ft_set_pipe_signals(void)
{
	signal(SIGTERM, ft_signal_pipe_handler);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
}
