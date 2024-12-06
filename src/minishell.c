/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:31:41 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/12/05 23:07:41 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp);
int	ft_handle_and_tokenize_input(t_minishell *ms);

int	main(int argc, char **argv, char **envp)
{
	t_minishell	ms;

	(void)argv;
	(void)argc;
	ms.exit_code = 0;
	ft_ms_struct(&ms, 0);
	ms.sa.sa_handler = ft_signal_handler;
	sigemptyset(&ms.sa.sa_mask);
	ms.sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &ms.sa, NULL);  // Ctrl-C
	sigaction(SIGQUIT, &ms.sa, NULL); // Ctrl-'\'
	ms.env.envp = ft_duplicate_envp(envp);
	if (!ms.env.envp)
		return (perror("malloc"), 1);
	while (1)
	{
		ft_set_main_signals();
		ms.save_stdin = dup(STDIN_FILENO);
		ms.save_stdout = dup(STDOUT_FILENO);
		ms.swap_input_redirects = false;
		ms.swap_output_redirects = false;
		if (ms.save_stdin == -1 || ms.save_stdout == -1)
			return(perror("dup"), 1);
		ms.input = readline(RD"minishell"RST"$ ");
		if (ms.input == NULL) // Ctrl-D ou EOF
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (*ms.input)
			add_history(ms.input);
		// Verifica sinais recebidos
		if (ft_handle_and_tokenize_input(&ms)) // Alterar o que faz e o erro!
		{
			fprintf(stderr, "unclosed quotes\n");
			continue ;
		}		
		ms.ast_root = ft_parse_ast(ms.tokens);
		// print_ast(ms.ast_root, 5); // Para testar a estrutura da AST
		if (ms.ast_root)
		{
			ms.status = ft_execute_ast(ms.ast_root, &ms);
			// if (ms.status != 0)
			// 	fprintf(stderr, "Erro na execução do comando: status %d\n", ms.status);
			if (dup2(ms.save_stdin, STDIN_FILENO) == -1 || dup2(ms.save_stdout, STDOUT_FILENO) == -1)
			{
				perror("dup2");
				close(ms.save_stdin);
				close(ms.save_stdout);
				return (1);
			}
			close(ms.save_stdin);
			close(ms.save_stdout);
		}
		ft_free_tokens(ms.tokens);
		ft_free_ast(ms.ast_root);
		free(ms.input);
/* 		if (g_sig_received == 1)
		{
			ms.exit_code = 148;
			g_sig_received = 0; // Ver como fazer!!
		} */
	}
	free(ms.input);
	ft_free_split(ms.env.envp);
	return (0);
}

int	ft_handle_and_tokenize_input(t_minishell *ms)
{
	ms->n_args = ft_count_args(ms->input); // Atualiza n_args diretamente em ms
	if (ms->n_args == -1)
		return (1);
	ms->tokens = ft_tokenize_input(ms->input, ms->n_args, 0, 0);
	ft_revalue_token_variable(ms);
	return (0);
}
