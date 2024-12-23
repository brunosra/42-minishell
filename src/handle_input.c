/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:39:44 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/12/23 19:42:34 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
extern volatile sig_atomic_t g_interrupt;

int			ft_count_args(char *str);
int			ft_skip_operator(const char *str, int i);
int			ft_check_quotes(char *str);
int			ft_handle_quotes(char *str, int i, int *start, int *end);
static int	ft_skip_and_process(char *str, int i, char quote_type);
int			ft_handle_argument(char *str, int i);

int	ft_skip_operator(const char *str, int i)
{
	if (str[i] == '|' || str[i] == '<' || str[i] == '>')
	{
		i++;
		if (str[i + 1] && (str[i] == '<' || str[i] == '>'))
			i++;
	}
	return (i);
}

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
		if (str[i] == '|' || str[i] == '<' || str[i] == '>')
		{
			i++;
			if (str[i] && str[i + 1] && (str[i] == '<' || str[i] == '>'))
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

int	ft_handle_argument(char *str, int i)
{
	while (str[i] && str[i] != ' ' && str[i] != '|'
				&& str[i] != '>' && str[i] != '<')
		i++;
	return (i);

}
