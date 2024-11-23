/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:39:44 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/11/23 04:03:23 by tcosta-f         ###   ########.fr       */
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
		while (str[i] == ' ')
			i++;
		if (str[i] == '\0')
			break ;
		if (str[i] == '|' || str[i] == '<' || str[i] == '>')
		{
			if (str[i + 1])
				i++;
			if (str[i + 1] && (str[i] == '<' || str[i] == '>'))
				i++;
			else if (!str[i + 1])
				i++;
			
		}
		else if (str[i] == '"' || str[i] == '\'')
		{
			i = ft_handle_quotes(str, i, 0, 0);
			if (i == -1)
				return (-1);
		}
		else
			i = ft_handle_argument(str, i); // avançar o índice no argumento
		count++;
	}
	printf("%i\n", count); // retirar no final
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

int ft_handle_quotes(char *str, int i, int *start, int *end)
{
    char single_quote = '\'';
    char double_quote = '"';
    char quote_type;

    if (start)
        *start = i;
    while (str[i])
	{
        if (str[i] == single_quote || str[i] == double_quote)
		{
            quote_type = str[i]; // Identifica o tipo de aspa (simples ou dupla)
            i++; // Avança após a aspa de abertura
			if (str[i] && str[i] == quote_type)
			{
				i++;
				continue ;
			}			
            while (str[i] && str[i] != quote_type) // Vai até o final do conteúdo entre as aspas
                i++;
            if (str[i] == quote_type)
			{
                i++; // Avança para passar a aspa de fechamento
                if (str[i] == ' ' || str[i] == '\0') // Se o próximo caractere for um espaço ou NULL, podemos parar
                    break ;
				if (str[i] && str[i] != ' ' && str[i] != single_quote && str[i] != double_quote)
				{
					while (str[i] && str[i] != ' ' && str[i] != single_quote && str[i] != double_quote)
						i++;
				}
				if (str[i] && str[i] == ' ')
					break;
            }
        }
		else
            i++; // Avança caso não seja uma aspa
    }
    if (end)
        *end = i;
    return (i);
}

int	ft_handle_argument(char *str, int i)
{
	while (str[i] && str[i] != ' ' && str[i] != '"' && str[i] != '\'' && str[i] != '|' && str[i] != '>' && str[i] != '<')
		i++; // Avança até o próximo espaço ou aspa
	return (i);
}
