/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:49:34 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/02/26 07:22:36 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token			*ft_tokenize_input(char *str, int n_args, int i, int j);
int				ft_tokenize(char *str, int *i, t_token *tokens, int *j);
static int		ft_handle_quoted_token(char *str, int *i, t_token *tokens, 
										int *j, t_type prev_type);
static void		ft_handle_operator_token(char *str, int *i);
static void		ft_handle_regular_token(char *str, int *i);
t_type			ft_get_token_type(char *str, t_type prev_type);
static t_type	ft_check_redirection_and_operator(char *str, t_type prev_type,
										bool *inverted);
static t_type	ft_check_redirection(char *str);
static t_type	ft_check_operator_or_exception(char *str);
static t_type	ft_check_variable(char *str);
int				ft_verify_variable_value(char *str, int i);
char			*ft_revalue_quoted_value(char *value);
static char		*ft_process_quoted_segment(char *value, int *i);
static char		*ft_process_unquoted_segment(char *value, int *i);
static void		ft_append_to_final(char **final, char *segment);

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

/**
 * @brief Tokenizes a segment of the input string into a token.
 * 
 * @param str     Input string.
 * @param i       Pointer to the current index in the string.
 * @param tokens  Array of tokens.
 * @param j       Pointer to the index in the token array.
 * @return int    Updated index after tokenization.
 */
int	ft_tokenize(char *str, int *i, t_token *tokens, int *j)
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
 * @brief  Checks if a string corresponds to a supported builtin command.
 * 
 * @param  str  Command string to check.
 * @return int
 **        1 if the command is a builtin.
 **        0 otherwise.
 */
int ft_check_builtins(char *str)
{
	if (!ft_strcmp(str, "echo"))
		return (1);
	if (!ft_strcmp(str, "pwd"))
		return (1);
	if (!ft_strcmp(str, "exit"))
		return (1);
	if (!ft_strcmp(str, "cd"))
		return (1);
	if (!ft_strcmp(str, "env"))
		return (1);
	if (!ft_strcmp(str, "export"))
		return (1);
	if (!ft_strcmp(str, "unset"))
		return (1);
	return (0);
}

/**
 * @brief Checks if the token is a redirection operator.
 * 
 * @param str Token value.
 * @return t_type Corresponding token type or TKN_CMD.
 */
static t_type	ft_check_redirection(char *str)
{
	if (!ft_strcmp(str, "<"))
		return (TKN_IN_RD);
	else if (!ft_strcmp(str, ">") || !ft_strcmp(str, ">>")
			|| !ft_strcmp(str, ">|"))
		return (TKN_OUT_RD);
	else if (!ft_strcmp(str, "<<"))
		return (TKN_HDOC);
	return (TKN_CMD);
}

/**
 * @brief Checks if the token is an operator or exception type.
 * 
 * @param str Token value.
 * @return t_type Corresponding token type or TKN_CMD.
 */
static t_type	ft_check_operator_or_exception(char *str)
{
	if (!ft_strcmp(str, "|"))
		return (TKN_PIPE);
	else if (!ft_strcmp(str, "||") || !ft_strcmp(str, "&&") || !ft_strcmp(str, ";") || !ft_strcmp(str, "&") || str[0] == ';' || str[0] == '&' || (str[0] == '|' && str[1]))
		return (TKN_EXCPT);
	return (TKN_CMD);
}

/**
 * @brief Determines if the token is a variable or filename type.
 * 
 * @param str       Token value.
 * @param prev_type Previous token type.
 * @return t_type Corresponding token type or TKN_ARG.
 */
static t_type	ft_check_variable(char *str)
{
	if (str[0] == '$')
		return (TKN_VAR);
	if (ft_verify_variable_value(str, 0))
		return (TKN_VAR);
	return (TKN_ARG);
}

/**
//  * @brief Determines the type of a token based on its value and the previous token's type.
//  * 
//  * @param str       Token value.
//  * @param prev_type Previous token type.
//  * @return t_type Corresponding token type.
//  */
// t_type	ft_get_token_type(char *str, t_type prev_type)
// {
// 	t_type type;
// 	static bool inverted = false;

// 	type = ft_check_redirection(str);
// 	if ((type == TKN_OUT_RD || type == TKN_HDOC) && (prev_type == TKN_PIPE || prev_type == TKN_NULL))
// 		inverted = true;
// 	if (type != TKN_CMD)
// 		return (type);
// 	if (prev_type == TKN_OUT_RD || prev_type == TKN_IN_RD
// 		|| prev_type == TKN_HDOC)
// 		return (TKN_FILE);
// 	type = ft_check_operator_or_exception(str);
// 	if (type == TKN_PIPE && (prev_type == TKN_PIPE || prev_type == TKN_EXCPT))
// 		return (TKN_EXCPT);
// 	if (prev_type == TKN_NULL || prev_type == TKN_PIPE || (prev_type == TKN_FILE && inverted == true))
// 	{
// 		inverted = false;
// 		if (ft_check_builtins(str) /* && (prev_type != TKN_CMD || prev_type != TKN_EXCPT) */)
// 			return (TKN_BLTIN);
// 		return(TKN_CMD);
// 	}
// 	if (type != TKN_CMD)
// 		return (type);
// 	if (str && (str[0] == '$' || ft_strchr(str, '$')))
// 		return (ft_check_variable(str));
// 	if ((str[0] == '"' || str[0] == '\'') &&
// 		(prev_type != TKN_CMD && prev_type != TKN_BLTIN &&
// 		 prev_type != TKN_VAR && prev_type != TKN_PIPE &&
// 		 prev_type != TKN_EXCPT && prev_type != TKN_ARG && prev_type != TKN_FILE))
// 		return (TKN_FILE);
// 	if (prev_type == TKN_CMD || prev_type == TKN_BLTIN ||
// 		prev_type == TKN_ARG || prev_type == TKN_VAR ||
// 		prev_type == TKN_FILE)
// 		return (TKN_ARG);
// 	return (TKN_CMD);
// }

/**
 * @brief  Handles redirection and operator token types.
 * 
 * @param  str        Token value.
 * @param  prev_type  Previous token type.
 * @param  inverted   Pointer to the static boolean flag.
 * @return t_type     Token type.
 */
static t_type	ft_check_redirection_and_operator(char *str, t_type prev_type,
	bool *inverted)
{
	t_type	type;

	type = ft_check_redirection(str);
	if ((type == TKN_OUT_RD || type == TKN_HDOC) &&
		(prev_type == TKN_PIPE || prev_type == TKN_NULL))
		*inverted = true;
	if (type != TKN_CMD)
		return (type);
	if (prev_type == TKN_OUT_RD || prev_type == TKN_IN_RD ||
		prev_type == TKN_HDOC)
	return (TKN_FILE);
		type = ft_check_operator_or_exception(str);
	if (type == TKN_PIPE &&	(prev_type == TKN_PIPE || prev_type == TKN_EXCPT))
		return (TKN_EXCPT);
	return (type);
}

/**
* @brief  Determines the type of a token based on its value and previous token.
* 
* @param  str        Token value.
* @param  prev_type  Previous token type.
* @return t_type     Token type.
*/
t_type	ft_get_token_type(char *str, t_type prev_type)
{
	t_type			type;
	static bool		inverted = false;

	type = ft_check_redirection_and_operator(str, prev_type, &inverted);
	if (type != TKN_CMD)
		return (type);
	if (prev_type == TKN_NULL || prev_type == TKN_PIPE ||
				(prev_type == TKN_FILE && inverted == true))
	{
		inverted = false;
		if (ft_check_builtins(str))
			return (TKN_BLTIN);
		return (TKN_CMD);
	}
	if (str && (str[0] == '$' || ft_strchr(str, '$')))
		return (ft_check_variable(str));
	if ((str[0] == '"' || str[0] == '\'') && (prev_type != TKN_CMD
		&& prev_type != TKN_BLTIN && prev_type != TKN_VAR 
		&& prev_type != TKN_PIPE && prev_type != TKN_EXCPT
		&& prev_type != TKN_ARG && prev_type != TKN_FILE))
		return (TKN_FILE);
	if (prev_type == TKN_CMD || prev_type == TKN_BLTIN || prev_type == TKN_ARG
		|| prev_type == TKN_VAR || prev_type == TKN_FILE)
		return (TKN_ARG);
	return (TKN_CMD);
}


/**
 * @brief  Verifies whether a variable within a string should be expanded.
 * 
 * @param  str  Input string.
 * @param  i  Index.
 * @return int
 **        1 if the variable should be expanded.
 **        0 otherwise.
 */
int	ft_verify_variable_value(char *str, int i)
{
	char	quote_type;
	int		expand;

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

/**
 * @brief Processes and extracts a quoted substring from a given value.
 * 
 * @param value Original string containing quotes.
 * @param i Current index position within the string.
 * @return char* The processed substring inside the quotes.
 */
static char	*ft_process_quoted_segment(char *value, int *i)
{
	char	quote_type;
	char	*start;
	char	*end;
	char	*arg;

	quote_type = value[*i];
	start = &value[*i];
	(*i)++;
	if (value[*i] == quote_type)
	{
		(*i)++;
		return (ft_strdup(""));
	}
	while (value[*i] && value[*i] != quote_type)
		(*i)++;
	if (value[*i] == quote_type)
		(*i)++;
	end = &value[*i];
	arg = ft_substr(value, start - value, end - start);
	return (arg);
}

/**
 * @brief Processes and extracts a substring outside of quotes.
 * 
 * @param value Original string.
 * @param i Current index position within the string.
 * @return char* The processed substring outside of the quotes.
 */
static char	*ft_process_unquoted_segment(char *value, int *i)
{
	char	*start;
	char	*end;
	char	*arg;

	start = &value[*i];
	while (value[*i] && value[*i] != '"' && value[*i] != '\'')
		(*i)++;
	end = &value[*i];
	arg = ft_substr(value, start - value, end - start);
	return (arg);
}

/**
 * @brief Appends a segment to the final string, handling memory management.
 * 
 * @param final Pointer to the final result string.
 * @param segment The segment to append.
 */
static void	ft_append_to_final(char **final, char *segment)
{
	char	*temp;

	if (!(*final))
		*final = ft_strdup(segment);
	else
	{
		temp = *final;
		*final = ft_strjoin(*final, segment);
		free(temp);
	}
	free(segment);
}

/**
 * @brief Processes a string containing quoted and unquoted segments, removing quotes and combining parts.
 * 
 * @param value Original string to process.
 * @return char* Processed string with quotes removed.
 */
char	*ft_revalue_quoted_value(char *value)
{
	int		i;
	char	*final;
	char	*segment;

	i = 0;
	final = NULL;
	if (!value)
		return (NULL);
	while (value[i])
	{
		if (value[i] == '"' || value[i] == '\'')
			segment = ft_process_quoted_segment(value, &i);
		else
			segment = ft_process_unquoted_segment(value, &i);
		ft_append_to_final(&final, segment);
	}
	free(value);
	return (final);
}

