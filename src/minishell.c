/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:31:41 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/11/08 03:01:30 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv);
static t_token *ft_handle_and_tokenize_input(char *input);

int	main(int argc, char **argv)
{
	char 				*input;
	struct	sigaction	sa;
	t_token				*tokens;

	while (42)
	{
		input = readline("minishell$ ");
		add_history(input);
		if (!input)
			break;
		tokens = ft_handle_and_tokenize_input(input);
	}
	sa.sa_handler = ft_signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	free(input);
}

static t_token *ft_handle_and_tokenize_input(char *input)
{
	int n_args;
	t_token *tokens;

	n_args = ft_count_args(input);
	if (n_args == -1)
		return (NULL);
	tokens = ft_tokenize_input(input, n_args, 0, 0);
	return (tokens);
}
