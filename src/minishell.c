/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:31:41 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/11/11 19:06:57 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int				main(void);
t_token *ft_handle_and_tokenize_input(char *input);

int	main(void)
{
	char 				*input;
	struct	sigaction	sa;
	t_token				*tokens;
	t_node				*ast_root;

	while (42)
	{
		input = readline("minishell$ ");
		add_history(input);
		if (!input)
			break;
		tokens = ft_handle_and_tokenize_input(input);
		ast_root = ft_parse_ast(tokens);
		print_ast(ast_root, 5);
		ft_execute_ast(ast_root);
	}
	sa.sa_handler = ft_signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	free(input);
	return (0);
} 
/*  // main para testes
int	main(void)
{
	char 				*input = "grep \"pattern\" \"output.txt\""; 
	t_token				*tokens;
	t_node				*ast_root;

	tokens = ft_handle_and_tokenize_input(input);
	ast_root = ft_parse_ast(tokens);
	print_ast(ast_root, 0);
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
