/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:39:44 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/11/21 06:00:22 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int			ft_count_args(char *str);
int			ft_check_quotes(char *str);
/* void		ft_get_additional_input(char **str); */
int			ft_handle_quotes(char *str, int i, int *start, int *end);
int			ft_handle_argument(char *str, int i);

int	ft_count_args(char *str)
{
	int count;
	int i;

	i = 0;
	count = 0;
 	if (ft_check_quotes(str))
		return (-1); // erro por falta de fechar aspas!
	while (str[i])
	{
		while (str[i] == ' ') // Ignora espaços
			i++;
		if (str[i] == '\0') // Final da string
			break ;
		if (str[i] == '|' || str[i] == '<' || str[i] == '>')
		{
			if (str[i + 1])
				i++;
			if (str[i + 1] && (str[i] == '<' || str[i] == '>'))
				i++;
		}
		else if (str[i] == '"' || str[i] == '\'') // Caso tenha aspas
		{
			i = ft_handle_quotes(str, i, 0, 0); // Trata as aspas
			if (i == -1)
				return (-1);
		}
		else
			i = ft_handle_argument(str, i); // Chama a função auxiliar para avançar o índice no argumento
		count++;
	}
	printf("%i\n", count);
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

/* void	ft_get_additional_input(char **str)
{
	char	*buffer;
	char	*new_str;

	buffer = readline("> ");
	if (buffer)
	{
		new_str = malloc(strlen(*str) + strlen(buffer) + 1);
		if (!new_str)
			exit(1);
		ft_strcpy(new_str, *str);
		ft_strcat(new_str, buffer);
		free(*str);
		*str = new_str;
		free(buffer);
	}
} */

int	ft_handle_quotes(char *str, int i, int *start, int *end)
{
	char	single_quote;
	char	double_quote;
	char	quote_type;

	quote_type = str[i];
	double_quote = '"';
	single_quote = '\'';
	if (str[i] && str[i] == quote_type)
	{
		while (str[i] && str[i] == quote_type)
			i++;
		if ((str[i] != quote_type && (str[i] == double_quote || str[i] == single_quote)) || (str[i] != quote_type && (str[i] != double_quote || str[i] != single_quote)))
		{
			if (start)
				*start = i;
			if (str[i] != quote_type && (str[i] == double_quote || str[i] == single_quote))
				i++;
		}
		while (str[i] && (str[i] == double_quote || str[i] == single_quote))
			i++;
		if (start && str[*start] != quote_type && str[*start] != double_quote && str[*start] != single_quote)
		{
			if (start)
				*start = i;
		}
	}
	while (str[i] && str[i] != quote_type )
		i++;
	if (end)
		*end = i;
	while (str[i] && (str[i] == double_quote || str[i] == single_quote))
	{
		if (str[i] != quote_type && (str[i] == double_quote || str[i] == single_quote) )
		{
			if (end)
				*end = i + 1;
		}
		i++;
	}
	return (i);
}

int	ft_handle_argument(char *str, int i)
{
	while (str[i] && str[i] != ' ' && str[i] != '"' && str[i] != '\'' && str[i] != '|' && str[i] != '>' && str[i] != '<')
		i++; // Avança até o próximo espaço ou aspa
	return (i);
}
