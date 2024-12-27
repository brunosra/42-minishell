/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:49:34 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/12/27 11:50:19 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
extern volatile sig_atomic_t g_interrupt;

t_token			*ft_tokenize_input(char *str, int n_args, int i, int j);
int				ft_tokenize(char *str, int *i, t_token *tokens, int *j);
t_type			ft_get_token_type(char *str, t_type prev_type);
char			*ft_revalue_quoted_value(char *value);
int				ft_verify_variable_value(char *str);

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
	tokens[j].type = TOKEN_NULL;
	return (tokens);
}

int	ft_tokenize(char *str, int *i, t_token *tokens, int *j)
{
	int		start;
	int		end;
	t_type	prev_type;

	start = *i;
	end = -1;
	if (*j > 0)
    	prev_type = tokens[*j - 1].type;
	else
    	prev_type = TOKEN_NULL;
	tokens[*j].old_value = NULL;
	if (str[*i] == '"' || str[*i] == '\'')
	{
		*i = ft_handle_quotes(str, *i, &start, &end);
		tokens[*j].value = ft_strndup(str + start, end - start);
		if (end - start == 2)
			tokens[*j].value = ft_strdup("\0");
		else
			tokens[*j].value = ft_revalue_quoted_value(tokens[*j].value);
		tokens[*j].type = ft_get_token_type(tokens[*j].value, prev_type);
		// printf("%s\n", tokens[*j].value);
		(*j)++;
		return (++(*i));
	}
 	else if (str[*i] == '|' || str[*i] == '>' || str[*i] == '<')
	{
		(*i)++;
		if (str[*i] == '>' || str[*i] == '<' || str[*i] == '|')
			(*i)++;
	}
	else
	{
		while (str[*i] && str[*i] != ' ' /* && str[*i] != '"' && str[*i] != '\'' */)
		{
			if (str[*i] == '|' || str[*i] == '>' || str[*i] == '<')
				break ;
			(*i)++;
		}	
	}
	tokens[*j].value = ft_strndup(str + start, *i - start);
	tokens[*j].type = ft_get_token_type(tokens[*j].value, prev_type);
	// printf("%s\n", tokens[*j].value);
	(*j)++;
	return (*i);
}

int ft_check_builtins(char *str)
{
	if (!ft_strcmp(str, "echo"))
		return (1);
	if (!ft_strcmp(str, "pwd"))
		return (/* printf("builtin: pwd -->"), */ 1);
	if (!ft_strcmp(str, "exit"))
		return (/* printf("builtin: exit -->"), */ 1);
	if (!ft_strcmp(str, "cd"))
		return (/* printf("builtin: cd -->"), */ 1);
	if (!ft_strcmp(str, "env"))
		return (/* printf("builtin: env -->"), */ 1);
	if (!ft_strcmp(str, "export"))
		return (/* printf("builtin: export -->"), */ 1);
	if (!ft_strcmp(str, "unset"))
		return (/* printf("builtin: export -->"), */ 1);
	return (0);
}

t_type	ft_get_token_type(char *str, t_type prev_type)
{
	if (!ft_strcmp(str, "<"))
		return (TOKEN_INPUT_REDIRECT);
	else if (!ft_strcmp(str, ">") || !ft_strcmp(str, ">>") || !ft_strcmp(str, ">|"))
		return (TOKEN_OUTPUT_REDIRECT);
	else if (!ft_strcmp(str, "<<"))
		return (TOKEN_HEREDOC);
	else if (!ft_strcmp(str, "|"))
		return (TOKEN_OPERATOR);
	else if (str && (str[0] == '$' || ft_strchr(str, '$')))
	{
		if (prev_type == TOKEN_OPERATOR || prev_type == TOKEN_NULL)
			return (TOKEN_COMMAND);
		else if (((str[0] == '"' || str[0] == '\'') && (prev_type != TOKEN_COMMAND && prev_type != TOKEN_BUILTIN && prev_type != TOKEN_VARIABLE && prev_type != TOKEN_OPERATOR && prev_type != TOKEN_EXCEPT && prev_type != TOKEN_ARGUMENT)) || (prev_type == TOKEN_OUTPUT_REDIRECT || prev_type == TOKEN_INPUT_REDIRECT || prev_type == TOKEN_HEREDOC))
			return (TOKEN_FILENAME);
		if (str[0] == '$')
			return (TOKEN_VARIABLE);
		if (ft_verify_variable_value(str))
			return (TOKEN_VARIABLE);
		else
			return (TOKEN_ARGUMENT);
	}
	else if (!ft_strcmp(str, "||") || !ft_strcmp(str, "&&"))
		return (TOKEN_EXCEPT); 
	else if (ft_check_builtins(str))
		return (TOKEN_BUILTIN);
	else if (prev_type == TOKEN_OPERATOR || prev_type == TOKEN_NULL) // TOKEN NULL mas + if ou mudar a posicao!
		return (TOKEN_COMMAND);
	else if ((prev_type == TOKEN_COMMAND || prev_type == TOKEN_BUILTIN || prev_type == TOKEN_ARGUMENT || prev_type == TOKEN_VARIABLE || prev_type == TOKEN_FILENAME))
		return (TOKEN_ARGUMENT);
	else if (((str[0] == '"' || str[0] == '\'') && (prev_type != TOKEN_COMMAND && prev_type != TOKEN_VARIABLE && prev_type != TOKEN_OPERATOR && prev_type != TOKEN_NULL)) || (prev_type == TOKEN_OUTPUT_REDIRECT || prev_type == TOKEN_INPUT_REDIRECT || prev_type == TOKEN_HEREDOC))
		return (TOKEN_FILENAME);
	return (TOKEN_COMMAND);
}

int	ft_verify_variable_value(char *str)
{
	int		i;
	char	quote_type;
	int		expand;

	i = 0;
	quote_type = '\0';
	expand = 0;

	while (str[i])
	{
		if ((str[i] == '"' || str[i] == '\''))
		{
			if (quote_type == str[i])
				quote_type = '\0'; // Fecha aspas
			else if (quote_type == '\0')
				quote_type = str[i]; // Abre aspas
			i++;
			continue;
		}
		if (str[i] == '$')
		{
			if (quote_type != '\'') // Expande fora de aspas ou em aspas duplas
				expand = 1;
		}
		i++;
	}

	return (expand);
}



char	*ft_revalue_quoted_value(char *value)
{
	int		i;
	char	quote_type;
	char	*start;
	char	*end;
	char	*arg;
	char	*final;
	char	*temp;

	i = 0;
	final = NULL;
	temp = NULL;
	arg = NULL;
	end = NULL;
	start = NULL;
	if (!value)
		return (NULL);
	while (value[i])
	{
		if (value[i] == '"' || value[i] == '\'')
		{
			quote_type = value[i];
			start = &value[i];
			i++;
			if (value[i] == quote_type)
			{
				i++;
				continue ;
			}
			while (value[i] && value[i] != quote_type )
				i++;
			if (value[i] == quote_type)
				i++;
			end = &value[i];
			arg = ft_substr(value, start - value, end - start);
		}
		else // Processa texto fora das aspas
		{
			start = &value[i];
			while (value[i] && value[i] != '"' && value[i] != '\'')
				i++;
			end = &value[i];
			arg = ft_substr(value, start - value, end - start);
		}
		if (!final)
		{
			final = ft_strdup(arg);
			free(arg);
		}
		else
		{
			temp = final;
			final = ft_strjoin(final, arg);
			free(temp);
			free(arg);
		}
	}
	free(value);
	return (final);
}

