/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <bschwell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:49:34 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/11/16 10:38:15 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token			*ft_tokenize_input(char *str, int n_args, int i, int j);
int				ft_tokenize(char *str, int *i, t_token *tokens, int *j);
t_type			ft_get_token_type(char *str, t_type prev_type);

t_token	*ft_tokenize_input(char *str, int n_args, int i, int j)
{
	t_token	*tokens;
	int n_tokens;

	n_tokens = 0;
	tokens = malloc(sizeof(t_token) * (n_args + 1)); // Aloca espaço para os tokens
	if (!tokens)
		return (NULL);
	while (str[i])
	{
		while (str[i] == ' ') // Ignora espaços
			i++;
		if (str[i] == '\0') // Final da string
			break ;
		i = ft_tokenize(str, &i, tokens, &j);
		n_tokens++;
		if (n_tokens == n_args)
			break ;
	}
	tokens[j].value = NULL; // Finaliza com NULL
	return (tokens);
}

int	ft_tokenize(char *str, int *i, t_token *tokens, int *j)
{
	int		start;
	t_type	prev_type;

	start = *i;
	if (*j > 0)
    	prev_type = tokens[*j - 1].type;
	else
    	prev_type = TOKEN_VARIABLE;
	if (str[*i] == '"' || str[*i] == '\'') // Caso tenha aspas
	{
		*i = ft_handle_quotes(str, *i); // Trata as aspas
		if (*i == -1)
			return (-1);
 		start++; //tirar para conter as aspas
		(*i)--;
	//	write(1, &str[*i], 1);
		tokens[*j].value = ft_strndup(str + start, *i - start);
	//	write(1, tokens[*j].value, ft_strlen(tokens[*j].value));
		tokens[*j].type = ft_get_token_type(tokens[*j].value, prev_type);
		(*j)++;
		return (++(*i));
	}
	else
	{
		while (str[*i] && str[*i] != ' ' && str[*i] != '"' && str[*i] != '\'')
			(*i)++;
	}
	tokens[*j].value = ft_strndup(str + start, *i - start);
//	write(1, tokens[*j].value, ft_strlen(tokens[*j].value));
	tokens[*j].type = ft_get_token_type(tokens[*j].value, prev_type);
	(*j)++;
	return (*i);
}

t_type	ft_get_token_type(char *str, t_type prev_type)
{
	if (!ft_strcmp(str, "|"))
		return (TOKEN_OPERATOR);
	else if (!ft_strcmp(str, ">") || !ft_strcmp(str, ">>"))
		return (TOKEN_OUTPUT_REDIRECT);
	else if (!ft_strcmp(str, "<") || !ft_strcmp(str, "<<"))
		return (TOKEN_INPUT_REDIRECT);
	else if (str[0] == '$')
		return (TOKEN_VARIABLE);
	else if ((prev_type == TOKEN_COMMAND || prev_type == TOKEN_ARGUMENT))
		return (TOKEN_ARGUMENT);
	else if (((str[0] == '"' || str[0] == '\'') && prev_type != TOKEN_COMMAND) || prev_type == TOKEN_OUTPUT_REDIRECT)
		return (TOKEN_FILENAME);
	return (TOKEN_COMMAND);
}
