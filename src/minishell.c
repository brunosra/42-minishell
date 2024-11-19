/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:31:41 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/11/19 20:38:55 by tcosta-f         ###   ########.fr       */
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
	ms.sa.sa_handler = ft_signal_handler;
	sigemptyset(&ms.sa.sa_mask);
	ms.sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &ms.sa, NULL);
	ms.env.envp = ft_duplicate_envp(envp);
	if (!ms.env.envp)
		return (perror("malloc"), 1);
	while (1)
	{
		ms.save_stdin = dup(STDIN_FILENO);
		ms.save_stdout = dup(STDOUT_FILENO);
		if (ms.save_stdin == -1 || ms.save_stdout == -1)
			return(perror("dup"), 1);
		ms.input = readline("minishell$ ");
		if (ms.input == NULL) // Saída do loop ao receber EOF (Ctrl+D)
		{
    		printf("Recebido NULL de readline. Saindo...\n");
    		break;
		}
		add_history(ms.input);
		ft_handle_and_tokenize_input(&ms);
		ms.ast_root = ft_parse_ast(ms.tokens);
		print_ast(ms.ast_root, 5); // Para testar a estrutura da AST
		if (ms.ast_root)
		{
			ms.status = ft_execute_ast(ms.ast_root, &ms);
			if (ms.status != 0)
				fprintf(stderr, "Erro na execução do comando: status %d\n", ms.status);
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
		ft_update_envp(&ms, envp);
		ft_free_tokens(ms.tokens);
		ft_free_ast(ms.ast_root);
		free(ms.input);
	}
	free(ms.input);
	ft_free_split(ms.env.envp);
	return (0);
}

/* int	main(void)
{
	char				*input = "echo ola | grep ola";
	t_minishell			ms;

	ms.tokens = ft_handle_and_tokenize_input(input);
	ms.ast_root = ft_parse_ast(ms.tokens);
	print_ast(ms.ast_root, 0);
	ft_execute_ast(ms.ast_root);
	ft_free_tokens(ms.tokens);
	ft_free_ast(ms.ast_root);
	return (0);
}
*/

int	ft_handle_and_tokenize_input(t_minishell *ms)
{
	ms->n_args = ft_count_args(ms->input); // Atualiza n_args diretamente em ms
	if (ms->n_args == -1)
		return (1);
	ms->tokens = ft_tokenize_input(ms->input, ms->n_args, 0, 0);
	ft_revalue_token_variable(ms);
	return (0);
}
