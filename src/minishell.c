/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:31:41 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/12/08 05:26:05 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp);
int	ft_handle_and_tokenize_input(t_minishell *ms);
int	ft_process_input_and_execute(t_minishell *ms);
int	ft_is_invalid_input(char *input);


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
		ms.in_pipe = 0;
		if (ms.save_stdin == -1 || ms.save_stdout == -1)
			return(perror("dup"), 1);
		ms.input = readline(RD"minishell"RST"$ ");
		if (ms.input == NULL) // Ctrl-D ou EOF
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (*ms.input && !ft_is_invalid_input(ms.input))
			add_history(ms.input);
		if (ft_process_input_and_execute(&ms))
			continue;
		ft_free_tokens(ms.tokens);
		ft_free_ast(ms.ast_root);
		free(ms.input);
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

int	ft_process_input_and_execute(t_minishell *ms)
{
	if (ft_handle_and_tokenize_input(ms)) // Lidar com entrada e tokenização
	{
		if (ms->exit_code == 2)
			return (1);
		else
		{
			ft_putstr_fd("minishell: unclosed quotes\n", STDERR_FILENO);
			return (1);
		}
	}
 	ms->ast_root = ft_parse_ast(ms->tokens);
	// print_ast(ms->ast_root, 5); // Para testar a estrutura da AST, se necessário
	if (ms->ast_root)
	{
		ms->status = ft_execute_ast(ms->ast_root, ms);
		if (dup2(ms->save_stdin, STDIN_FILENO) == -1 || dup2(ms->save_stdout, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close(ms->save_stdin);
			close(ms->save_stdout);
			return (1);
		}
		if (ms->in_pipe != 0)
		{
			add_history(ms->input);
			ms->in_pipe--;
		}
		else
		{
			close(ms->save_stdin);
			close(ms->save_stdout);
		}
	}
	return (0);
}

int	ft_is_invalid_input(char *input)
{
	int	i;

	if (!input || *input == '\0') // Verifica se a entrada é vazia ou nula
		return (1);
	i = ft_strlen(input) - 1;
	while (i >= 0 && (input[i] == ' ' || input[i] == '\t')) 
		i--;
	if (i >= 0 && input[i] == '|') // Verifica se a entrada termina com '|'
	{
		if (input[i - 1] == '|')
			return (0);// Verifica se há algo válido antes do '|'
		while (--i >= 0)
		{
			if (input[i] != ' ' && input[i] != '\t') // Algum caractere válido antes do '|'
				return (1); // Entrada válida
		}
	}
	return (0); // Entrada válida (não termina com `|`)
}
