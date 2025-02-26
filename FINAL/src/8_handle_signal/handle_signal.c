/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:57:36 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/02/26 06:38:42 by tcosta-f         ###   ########.fr       */
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
	t_minishell	*ms;

	ms = ft_ms_struct(NULL, 1);
	if (sig == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		ft_set_exit_code(ms, 130); // Atualiza o exit_code antes de recriar o prompt
		// ft_create_prompt(ms); // nao funcioma! nao atualiza novo prompt para 130!
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
	struct sigaction sa;

	sa.sa_handler = ft_signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;  // Reinicia chamadas interrompidas
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
	signal(SIGQUIT, SIG_IGN); // Se for para executar alterar para SIG_DFL
} 

/**
 * @brief Configures signal handling for heredoc input mode.
 * 
 * Ensures SIGINT interrupts the heredoc properly, while SIGQUIT is ignored.
 */
void	ft_set_heredoc_signals(void)
{
	signal(SIGINT, ft_signal_heredoc_handler);
	signal(SIGQUIT, SIG_IGN); // Ignora Ctrl-'\'
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
	t_minishell	*ms;

	ms = ft_ms_struct(NULL, 1);
	if (sig == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		ft_set_exit_code(ms, 130);
		exit(130);
	}
}
