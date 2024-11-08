/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:49:34 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/11/08 03:06:41 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*ft_tokenize_input(char *str, int n_args, int i, int j);
static int		ft_tokenize(char *str, int *i, t_token *tokens, int *j);
static t_type	ft_get_token_type(char *str);

t_token	*ft_tokenize_input(char *str, int n_args, int i, int j)
{
	t_token	*tokens;

	tokens = malloc(sizeof(t_token) * (n_args + 1)); // Aloca espaço para os tokens
	if (!tokens)
		return (NULL);

	while (str[i])
	{
		while (str[i] == ' ') // Ignora espaços
			i++;
		if (str[i] == '\0') // Final da string
			break ;
		if (str[i] == '"' || str[i] == '\'') // Caso tenha aspas
		{
			i = ft_handle_quotes(str, i); // Trata as aspas
			if (i == -1)
				return (NULL);
		}
		else
			i = ft_tokenize(str, &i, tokens, &j); // Chama a função auxiliar para tokenizar
	}
	tokens[j].value = NULL; // Finaliza com NULL
	return (tokens);
}

int	ft_tokenize(char *str, int *i, t_token *tokens, int *j)
{
	int start = *i;

	while (str[*i] && str[*i] != ' ' && str[*i] != '"' && str[*i] != '\'') // Vai até o próximo argumento
		(*i)++;
	tokens[*j].value = ft_strndup(str + start, *i - start); // Copia o argumento
	tokens[*j].type = ft_get_token_type(tokens[*j].value); // Determina o tipo do token
	(*j)++;
	return (*i);
}

t_type	ft_get_token_type(char *str)
{
	if (!ft_strcmp(str, "|") || !ft_strcmp(str, ">") || !ft_strcmp(str, ">>") || !ft_strcmp(str, "<"))
		return (TOKEN_OPERATOR);
	else if (str[0] == '$')
		return (TOKEN_VARIABLE);
	else if (str[0] == '-')
		return (TOKEN_ARGUMENT);
	else if (str[0] == '"' || str[0] == '\'') // Lidar com aspas, FALTA, PODE NAO TER ASPAS :( ver o tipo anterior para saber!
		return (TOKEN_FILENAME);
	return (TOKEN_COMMAND);
}
