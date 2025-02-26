/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_tokenize_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 00:41:24 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/02/26 06:38:42 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token		*ft_tokenize_input(char *str, int n_args, int i, int j);
static int	ft_tokenize(char *str, int *i, t_token *tokens, int *j);
static int	ft_handle_quoted_token(char *str, int *i, t_token *tokens, 
									int *j, t_type prev_type);
static void	ft_handle_operator_token(char *str, int *i);
static void	ft_handle_regular_token(char *str, int *i);

/**
 * @brief  Tokenizes the input string into an array of tokens.
 * 
 * @param  str      Input string to tokenize.
 * @param  n_args   Number of arguments in the input.
 * @param  i        Current index in the input string.
 * @param  j        Index to store tokens in the array.
 * @return t_token* Pointer to the array of tokens.
 */
t_token	*ft_tokenize_input(char *str, int n_args, int i, int j)
{
	t_token	*tokens;
	int 	n_tokens;

	n_tokens = 0;
	tokens = malloc(sizeof(t_token) * (n_args + 1));
	if (!tokens)
		return (NULL);
	while (str[i])
	{
		while (str[i] == ' ')
			i++;		
		if (str[i] == '\0')
			break ;
		i = ft_tokenize(str, &i, tokens, &j);
		n_tokens++;
		if (n_tokens == n_args)
			break ;
	}
	tokens[j].value = NULL;
	tokens[j].type = TKN_NULL;
	return (tokens);
}

/**
 * @brief Tokenizes a segment of the input string into a token.
 * 
 * @param str     Input string.
 * @param i       Pointer to the current index in the string.
 * @param tokens  Array of tokens.
 * @param j       Pointer to the index in the token array.
 * @return int    Updated index after tokenization.
 */
static int	ft_tokenize(char *str, int *i, t_token *tokens, int *j)
{
	int		start;
	t_type	prev_type;

	start = *i;
	if (*j > 0)
		prev_type = tokens[*j - 1].type;
	else
		prev_type = TKN_NULL;
	tokens[*j].old_value = NULL;
	if (str[*i] == '"' || str[*i] == '\'')
		return (ft_handle_quoted_token(str, i, tokens, j, prev_type));
	if (str[*i] == '|' || str[*i] == '>' || str[*i] == '<' || str[*i] == ';' || str[*i] == '&')
		ft_handle_operator_token(str, i);
	else
		ft_handle_regular_token(str, i);
	tokens[*j].value = ft_strndup(str + start, *i - start);
	tokens[*j].type = ft_get_token_type(tokens[*j].value, prev_type);
	*j += 1;
	return (*i);
}

/**
 * @brief Handles tokens that are enclosed in quotes.
 * 
 * @param str         Input string.
 * @param i           Pointer to the current index in the string.
 * @param tokens      Array of tokens.
 * @param j           Pointer to the index in the token array.
 * @param prev_type   Type of the previous token.
 * @return int        Updated index after processing the quoted token.
 */
static int	ft_handle_quoted_token(char *str, int *i, t_token *tokens, int *j, 
									t_type prev_type)
{
	int	start;
	int	end;

	start = *i;
	*i = ft_handle_quotes(str, *i, &start, &end);
	tokens[*j].value = ft_strndup(str + start, end - start);
	if ((end - start) == 2)
		tokens[*j].value = ft_strdup("\0");
	else
		tokens[*j].value = ft_revalue_quoted_value(tokens[*j].value);
	tokens[*j].type = ft_get_token_type(tokens[*j].value, prev_type);
	*j += 1;
	*i += 1;
	return (*i);
}

/**
 * @brief Handles tokens that are operators like |, >, <.
 * 
 * @param str   Input string.
 * @param i     Pointer to the current index in the string.
 */
static void	ft_handle_operator_token(char *str, int *i)
{
	*i += 1;
	if (str[*i] == '>' || str[*i] == '<' || str[*i] == '|' || str[*i] == ';' || str[*i] == '&')
		*i += 1;
}

/**
 * @brief Handles regular tokens that are not quoted or operators.
 * 
 * @param str   Input string.
 * @param i     Pointer to the current index in the string.
 */
static void	ft_handle_regular_token(char *str, int *i)
{
	while (str[*i] && str[*i] != ' ')
	{
		if (str[*i] == '|' || str[*i] == '>' || str[*i] == '<' || str[*i] == ';' || str[*i] == '&')
			break;
		*i += 1;
	}
}

