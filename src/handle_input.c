/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:39:44 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/02/19 16:59:38 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
extern volatile int	g_interrupt;

int			ft_count_args(char *str);
int			ft_skip_operator(const char *str, int i);
int			ft_check_quotes(char *str);
int			ft_handle_quotes(char *str, int i, int *start, int *end);
static int	ft_skip_and_process(char *str, int i, char quote_type);
int			ft_handle_argument(char *str, int i);

/**
 * @brief  Skip an operator in the input string, advancing the index.
 * 
 * @param  str  Input string.
 * @param  i    Current index in the string.
 * @return int  Updated index after skipping the operator.
 */
int	ft_skip_operator(const char *str, int i)
{
	if (str[i] == '|' || str[i] == '<' || str[i] == '>' || str[i] == ';' || str[i] == '&')
	{
		i++;
		if (str[i + 1] && (str[i] == '<' || str[i] == '>' || str[i] == '&'))
			i++;
	}
	return (i);
}

/**
 * @brief  Count the number of arguments in the input string.
 * 
 * @param  str  Input string.
 * @return int
 **        Number of arguments found in the string.
 */
int	ft_count_args(char *str)
{
	int count;
	int i;

	i = 0;
	count = 0;

	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (str[i] == '\0')
			break ;
		if (str[i] == '|' || str[i] == '<' || str[i] == '>' || str[i] == ';' || str[i] == '&')
		{
			i++;
			if (str[i] && str[i + 1] && (str[i] == '<' || str[i] == '>' || str[i] == '&'))
				i++;
		}
		else if (str[i] == '"' || str[i] == '\'')
			i = ft_handle_quotes(str, i, 0, 0);
		else
			i = ft_handle_argument(str, i);
		count++;
	}
	// printf("%i\n", count); // retirar no final
	return (count);
}

/**
 * @brief  Check if the input string contains unclosed quotes.
 * 
 * @param  str  Input string.
 * @return int
 **        0 if no unclosed quotes are found
 **        1 if unclosed quotes are detected
 */
int	ft_check_quotes(char *str)
{
	int		i;
	char	quote_type;

	i = 0;
	quote_type = '\0';
	while (str[i])
	{
		if ((str[i] == '"' || str[i] == '\'') && quote_type == '\0')
			quote_type = str[i];
		else if (str[i] == quote_type)
			quote_type = '\0';
		i++;
	}
	if (quote_type != '\0')
		return (1);
	return (0);
}

/**
 * @brief  Skip content within quotes and process the rest of the string.
 * 
 * @param  str          Input string.
 * @param  i            Current index in the string.
 * @param  quote_type   The type of quote (' or ").
 * @return int          Updated index after processing quotes.
 */
static int	ft_skip_and_process(char *str, int i, char quote_type)
{
	while (str[i] && str[i] != quote_type)
		i++;
	if (str[i] == quote_type)
		i++;
	if (str[i] == ' ' || str[i] == '\0')
		return (i);
	while (str[i] && str[i] != ' ' && str[i] != '\'' && str[i] != '"')
		i++;
	return (i);
}

/**
 * @brief  Handle quoted sections in the input string.
 * 
 * @param  str    Input string.
 * @param  i      Current index in the string.
 * @param  start  Pointer to store the start index of the quoted section (optional).
 * @param  end    Pointer to store the end index of the quoted section (optional).
 * @return int    Updated index after handling the quoted section.
 */
int	ft_handle_quotes(char *str, int i, int *start, int *end)
{
	char	quote_type;

	if (start)
		*start = i;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			quote_type = str[i++];
			if (str[i] && str[i] == quote_type && str[i - 2] != '$')
				i++;
			else
				i = ft_skip_and_process(str, i, quote_type);
			if (str[i] == ' ')
				break;
		}
		else
			i++;
	}
	if (end)
		*end = i;
	return (i);
}

/**
 * @brief  Handle a non-quoted argument in the input string.
 * 
 * @param  str  Input string.
 * @param  i    Current index in the string.
 * @return int  Updated index after handling the argument.
 */
int	ft_handle_argument(char *str, int i)
{
	while (str[i] && str[i] != ' ' && str[i] != '|'
				&& str[i] != '>' && str[i] != '<' && str[i] != ';' && str[i] != '&')
		i++;
	return (i);
}
