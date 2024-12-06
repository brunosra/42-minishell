/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:57:36 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/12/06 02:41:37 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_signal_handler(int sig);
t_minishell	*ft_ms_struct(t_minishell *ms, int flag);
void	ft_set_main_signals(void);
void	ft_set_fork_signals(void);
void	ft_set_heredoc_signals(void);
void	ft_signal_heredoc_handler(int sig);

// void	ft_check_signals();

void	ft_signal_handler(int sig)
{
	t_minishell	*ms;

	ms = ft_ms_struct(NULL, 1);
	if (sig == SIGINT) // Ctrl-C
	{
		write(STDERR_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		ms->exit_code = 130; // Código típico para interrupções via Ctrl-C
	}
/* 	else if (sig == SIGQUIT) // Ctrl-'\'
	{
		write(STDOUT_FILENO, "\b\b  \b\b", 6); // Remove o visual `^\`
	}
 */}

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

void	ft_set_main_signals(void)
{
	signal(SIGINT, ft_signal_handler);
	signal(SIGQUIT, SIG_IGN); // Se for para executar alterar para SIG_DFL
}

void	ft_set_fork_signals(void)
{
	signal(SIGINT, SIG_IGN); 
	signal(SIGQUIT, SIG_IGN); // Se for para executar alterar para SIG_DFL
} 

/* void	ft_set_fork_signals(void)
{
	signal(SIGINT, SIG_IGN); 
	signal(SIGQUIT, SIG_IGN); // Se for para executar alterar para SIG_DFL
}  */

void	ft_set_heredoc_signals(void)
{
	signal(SIGINT, ft_signal_heredoc_handler);
	signal(SIGQUIT, SIG_IGN); // Ignora Ctrl-'\'
}

void	ft_signal_heredoc_handler(int sig)
{
	t_minishell	*ms;

	ms = ft_ms_struct(NULL, 1);
	if (sig == SIGINT) // Ctrl-C no heredoc
	{
		write(STDOUT_FILENO, "\n", 1);
		close(ms->pipefd[0]); // Fecha o arquivo do heredoc
/* 		free(ms->input); // Libera recursos
		ft_free_ast(ms->ast_root); */
		exit(130); // Sai com o código 130
	}
}
