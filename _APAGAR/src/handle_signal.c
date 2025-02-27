/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:57:36 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/02/26 04:51:44 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		ft_signal_handler(int sig);
t_minishell	*ft_ms_struct(t_minishell *ms, int flag);
void		ft_set_main_signals(void);
void		ft_set_fork_signals(void);
void		ft_set_heredoc_signals(void);
void		ft_signal_heredoc_handler(int sig);

// void	ft_check_signals();

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
	if (sig == SIGINT) // Ctrl-C
	{
		write(STDERR_FILENO, "\n", 1);
		ft_set_exit_code(ms, 130); // Atualiza o exit_code antes de recriar o prompt
		// ft_create_prompt(ms); // Atualiza o prompt com o novo exit_code / nao funcioma! nao atualiza novo prompt para 130!
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay(); // Agora reflete o novo prompt atualizado
	}
}

/**
 * @brief Manages the global minishell structure.
 * 
 * If the flag is set, it returns the stored pointer. Otherwise, it updates
 * the stored pointer to the given minishell structure.
 * 
 * @param ms    Pointer to the minishell structure (if updating).
 * @param flag  If nonzero, retrieves the stored pointer.
 * @return t_minishell*  The global minishell structure pointer.
 */
t_minishell	*ft_ms_struct(t_minishell *ms, int flag)
{
	static t_minishell	*ptr;

	if (flag)
		return (ptr); // Retorna o ponteiro armazenado
	ptr = ms; // Atualiza o ponteiro
	return (ptr);
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
	if (sig == SIGINT) // Ctrl-C
	{
		write(STDERR_FILENO, "\n", 1);
		ft_set_exit_code(ms, 130);
		exit(130); // Código típico para interrupções via Ctrl-C
	}
}
