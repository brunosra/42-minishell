/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:31:41 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/11/16 15:07:35 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int				main(void);
int	ft_handle_and_tokenize_input(t_minishell *ms);

int	main(void)//int argc, char **argv, char **envp     //extern type environ
{
	t_minishell	ms;

	// Configuração inicial do signal handler
	ms.sa.sa_handler = ft_signal_handler;
	sigemptyset(&ms.sa.sa_mask);
	ms.sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &ms.sa, NULL);

	while (1)
	{
		// Salva os descritores originais de stdin e stdout
		ms.save_stdin = dup(STDIN_FILENO);
		ms.save_stdout = dup(STDOUT_FILENO);
		if (ms.save_stdin == -1 || ms.save_stdout == -1)
		{
			perror("dup");
			return (1);
		}

		// Lê o comando do usuário
		ms.input = readline("minishell$ ");
		if (ms.input == NULL) // Saída do loop ao receber EOF (Ctrl+D)
		{
    		printf("Recebido NULL de readline. Saindo...\n");
    		break;
		}
		add_history(ms.input);

		// Tokeniza e cria a AST
		ft_handle_and_tokenize_input(&ms);
		ms.ast_root = ft_parse_ast(ms.tokens);

		print_ast(ms.ast_root, 5); // Para testar a estrutura da AST

		// Executa a AST, se válida
		if (ms.ast_root)
		{
			ms.status = ft_execute_ast(ms.ast_root, &ms);
			if (ms.status != 0)
				fprintf(stderr, "Erro na execução do comando: status %d\n", ms.status);

			// Restaura stdin e stdout para o estado original
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

		// Libera memória
		ft_free_tokens(ms.tokens);
		ft_free_ast(ms.ast_root);
		free(ms.input);
	}

	// Libera o último input antes de sair
	free(ms.input);
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
	return (0);
}
