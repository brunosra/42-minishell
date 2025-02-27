/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:57:36 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/02/27 19:21:03 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_signal_handler(int sig);
void	ft_set_main_signals(void);
void	ft_set_fork_signals(void);
void	ft_set_heredoc_signals(void);
void	ft_signal_heredoc_handler(int sig);

/**
 * @brief Handles the reception of system signals (SIGINT).
 * 
 * This function updates the shell's exit code upon receiving Ctrl-C (SIGINT),
 * clears the input line, and redisplays the prompt.
 * 
 * @param sig  The received signal (expected SIGINT).
 */
void	ft_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		ft_exit_code(130);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

/**
 * @brief Configures signal handling for the main minishell process.
 * 
 * This function ensures SIGINT is properly handled to update the prompt,
 * while ignoring SIGQUIT.
 */
void	ft_set_main_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = ft_signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

/**
 * @brief Configures signal handling for child processes.
 * 
 * In a forked process, SIGINT and SIGQUIT are ignored to prevent interruption
 * unless explicitly handled in execution.
 */
void	ft_set_fork_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

/**
 * @brief Configures signal handling for heredoc input mode.
 * 
 * Ensures SIGINT interrupts the heredoc properly, while SIGQUIT is ignored.
 */
void	ft_set_heredoc_signals(void)
{
	signal(SIGINT, ft_signal_heredoc_handler);
	signal(SIGQUIT, SIG_IGN);
}

/**
 * @brief Handles SIGINT inside a heredoc.
 * 
 * When Ctrl-C is pressed during a heredoc input, this function interrupts
 * input capture and exits with code 130.
 * 
 * @param sig  The received signal (expected SIGINT).
 */
void	ft_signal_heredoc_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		ft_exit_code(130);
		exit(130);
	}
}
