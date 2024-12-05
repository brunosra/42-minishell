/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:57:36 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/12/05 07:29:27 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_signal_handler(int sig);
// void	ft_check_signals();

void	ft_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(2, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_sig_received = 1;
		// ms -> exit_code = 139; 
	} 
	else if (sig == SIGQUIT)
	{
		// g_sig_received = 0; // Não faz nada, mas reseta o estado
		write(STDOUT_FILENO, "\b\b  \b\b", 6); // Remove o `^\` visual
	} // Ctrl-'\'
/* 	else if (sig == EOF)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		g_sig_received = 2;
		exit(0); // Sai do shell
	} 
 */
}

/* void	ft_check_signals()
{
	if (g_sig_received == 1) // Ctrl-C
	{
		write(2, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_sig_received = 0; // Reseta o estado
		// ms->exit_code = 130);
		exit(139);             // Reexibe o prompt
	}
	else if (g_sig_received == 2) // Ctrl-D
	{
		write(STDOUT_FILENO, "exit\n", 5);
		exit(0); // Sai do shell
	}
	else if (g_sig_received == 0)
	{
		g_sig_received = 0; // Não faz nada, mas reseta o estado
		write(STDOUT_FILENO, "\b\b  \b\b", 6); // Remove o `^\` visual

	}
	// Caso SIGQUIT (Ctrl-\), g_sig_received será 0, e não há ação.
}
 */