/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:31:41 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/11/13 03:43:46 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int				main(void);
t_token *ft_handle_and_tokenize_input(char *input);

int	main(void)
{
	char				*input;
	struct	sigaction	sa;
	t_token				*tokens;
	t_node				*ast_root;
	int					status;

	sa.sa_handler = ft_signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	while (1)
	{
		input = NULL;
		input = readline("minishell$ ");
		if (!input) // Saída do loop ao receber EOF (Ctrl+D)
		{
    		printf("Recebido NULL de readline. Saindo...\n");
    		break;
		}
		add_history(input);
		tokens = ft_handle_and_tokenize_input(input);
		ast_root = ft_parse_ast(tokens);
		print_ast(ast_root, 5); // para testar
		if (ast_root)
		{
			status = ft_execute_ast(ast_root);
			if (status != 0)
				fprintf(stderr, "Erro na execução do comando: status %d\n", status);
		}
		ft_free_tokens(tokens); // Libera tokens
		ft_free_ast(ast_root); // Libera AST
		free(input); // Libera a string do input
	}
	free(input); // Libera o último input antes de sair
	return (0);
}
 // main para testes
/* int	main(void)
{
	char 				*input = "echo ola | grep ola"; 
	t_token				*tokens;
	t_node				*ast_root;

	tokens = ft_handle_and_tokenize_input(input);
	ast_root = ft_parse_ast(tokens);
	print_ast(ast_root, 0);
	ft_execute_ast(ast_root);
	ft_free_tokens(tokens);
	ft_free_ast(ast_root);
	return (0);
} 
 */
t_token *ft_handle_and_tokenize_input(char *input)
{
	int n_args;
	t_token *tokens;

	n_args = ft_count_args(input);
	if (n_args == -1)
		return (NULL);
	tokens = ft_tokenize_input(input, n_args, 0, 0);
	return (tokens);
}
