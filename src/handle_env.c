/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:50:15 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/02/25 16:13:44 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		ft_revalue_tkn_var(t_minishell *ms);
static char	*ft_process_token_expansion(t_minishell *ms, t_token *token, char *ptr);
static int	ft_is_expandable_token(t_type type);
int		ft_check_balanced_quotes(char *str, int idx);
char	**ft_duplicate_envp(char **envp);
int	ft_check_if_expand(char *str, char *ptr, int heredoc);
static int	ft_verify_quotes(char *quote_type, char c);
static int	ft_check_expansion_conditions(char *str, int i, int heredoc, char quote_type);
static int	ft_is_invalid_dollar_char(char c);
int		ft_replace_str(char **value, char *key, char *ptr, char *env_value);
static int	ft_replace_entire_str(char **value, char *env_value);
int	ft_cleanup(char *to_free1, char *to_free2, int error);
static char	*ft_create_replaced_str(char *start, char *env_value, char *end);
int		ft_revalue_heredoc_input(char **input, t_minishell *ms);
static int	ft_process_heredoc_expansion(char **input, t_minishell *ms, char **ptr);
static void	ft_expand_exit_code(char **input, t_minishell *ms, char **ptr);
static int	ft_expand_env_var(char **input, t_minishell *ms, char **ptr);
char	*ft_get_env_value(const char *str, t_minishell *ms, char **key, bool heredoc);
char	*ft_get_env(const char *key, t_minishell *ms);

/**
 * @brief  Duplicates the environment variables array.
 * 
 * @param  envp  Array of environment variables to duplicate.
 * @return char**  Pointer to the newly allocated duplicate of `envp`.
 **         NULL if memory allocation fails.
 */
char	**ft_duplicate_envp(char **envp)
{
	int		i;
	char	**new_envp;

	i = 0;
	while (envp[i])
		i++;
	new_envp = malloc(sizeof(char *) * (i + 1));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			ft_free_split(new_envp);
			return (NULL);
		}
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

/**
 * @brief  Replaces tokens in the input with their corresponding environment variable values.
 * 
 * @param  ms  Pointer to the minishell structure containing tokens and environment variables.
 * @return int Status of the operation.
 **         0 on success.
 **         1 on error or if expansion fails.
 */
// int	ft_revalue_tkn_var(t_minishell *ms)
// {
// 	int		i;
// 	char	*env_value;
// 	char	*ptr;
// 	char	*key;

// 	i = -1;
// 	env_value = NULL;
// 	key = NULL;
// 	ptr = NULL;
// 	if (!ms || !ms->tokens || !ms->env.envp)
// 		return (1);
// 	while (ms->tokens[++i].value)
// 	{
// 		// ft_putstr_fd(ms->tokens[i].value, 1);
// 		if (ms->tokens[i].type == TKN_VAR || ms->tokens[i].type == TKN_CMD || ms->tokens[i].type == TKN_FILE || ms->tokens[i].type == TKN_ARG)
// 		{
// 			ptr = ft_strchr(ms->tokens[i].value, '$');
// 			while (ptr != NULL)
// 			{
// 				if (ft_check_if_expand(ms->tokens[i].value, ptr, 0) == 1)
// 				{
// 					env_value = ft_get_env_value(ptr, ms, &key);
// 					if (!env_value) 
//  						env_value = ft_strdup("");
// 					ft_replace_str(&ms->tokens[i].value, key, ptr, env_value);
// 					if (*key)
// 						free(key);
// 					if (!ms->tokens[i].value)
// 					{
// 						perror("");
// 						return (1);
// 					}
// 					// if (ms->tokens[i].value[0] == '\0' && ms->tokens[i].type == TKN_CMD)
// 					// 	ms->tokens[i].type = TKN_VAR; // alaterar para TOKEN_EMPTY para ser ignorado no parsing. outra e perceber a quando do tokenixe type se a expansao da var e nula e se assim for passa a TOKEN_EMPTY...
// 					ptr = ft_strchr(ms->tokens[i].value, '$');
// 					if (!ptr)
// 						break ;
// 				}
// 				else if (ft_check_if_expand(ms->tokens[i].value, ptr, 0) == 2)
// 				{
// 					key = ft_strdup("?");
// 					// printf("%lu\n", ft_strlen(ft_itoa(ft_exit_code(ms))));
// 					ft_replace_str(&ms->tokens[i].value, key, ptr, ft_itoa(ft_exit_code(ms)));
// 					free(key);
// 					ptr = ft_strchr(ms->tokens[i].value, '$');
// 					if (!ptr)
// 						break ;
// 				}
// 				else
// 				{
// 					ptr++;
// 					ptr = ft_strchr(ptr, '$');
// 					if (!ptr)
// 						break ;
// 				}
// 			}
// 		}
// 	}
// 	return (0);
// }

/**
 * @brief  Expands environment variables in tokens of type VAR, CMD, FILE, or ARG.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return int Status code.
 **         0 on success.
 **         1 on error.
 */
int	ft_revalue_tkn_var(t_minishell *ms)
{
	int	i;
	char	*ptr;

	if (!ms || !ms->tokens || !ms->env.envp)
		return (1);
	i = -1;
	while (ms->tokens[++i].value)
	{
		if (ft_is_expandable_token(ms->tokens[i].type) && ms->tokens[i - 1].type != TKN_HDOC)
		{
			ptr = ft_strchr(ms->tokens[i].value, '$');
			while (ptr && *ptr)
				ptr = ft_process_token_expansion(ms, &ms->tokens[i], ptr);
		}
	}
	return (0);
}

/**
 * @brief  Checks if a token type is eligible for expansion.
 * 
 * @param  type  Token type.
 * @return int   1 if expandable, 0 otherwise.
 */
static int	ft_is_expandable_token(t_type type)
{
	return (type == TKN_VAR || type == TKN_CMD || type == TKN_FILE || type == TKN_ARG);
}

/**
 * @brief  Expands a `$` variable inside a token.
 * 
 * @param  ms     Pointer to the minishell structure.
 * @param  token  Pointer to the token being processed.
 * @param  ptr    Pointer to the `$` inside the token value.
 * @return char*  Pointer to the next `$` to expand, or NULL if none remain.
 */
static char	*ft_process_token_expansion(t_minishell *ms, t_token *token, char *ptr)
{
	char	*env_value;
	char	*key;

	env_value = NULL;
	key = NULL;
	if (ft_check_if_expand(token->value, ptr, 0) == 1)
	{
		env_value = ft_get_env_value(ptr, ms, &key, false);
		if (!env_value)
			env_value = ft_strdup("");
		ft_replace_str(&token->value, key, ptr, env_value);
	}
	else if (ft_check_if_expand(token->value, ptr, 0) == 2)
	{
		key = ft_strdup("?");
		ft_replace_str(&token->value, key, ptr, ft_itoa(ft_exit_code(ms)));
	}
	else
	{
		ptr++;
		return (ft_strchr(ptr, '$'));
	}
	free(key);
	return (ft_strchr(token->value, '$'));
}


/**
 * @brief  Extracts the value of an environment variable from a given string.
 * 
 * @param  str  String containing the variable name (e.g., `$VAR`).
 * @param  ms   Pointer to the minishell structure.
 * @param  key  Pointer to store the extracted variable name.
 * @param heredoc Bool to indicate whether it is for a heredoc or not.
 * @return char*  Value of the environment variable.
 **         NULL if the variable does not exist.
 */
char	*ft_get_env_value(const char *str, t_minishell *ms, char **key, bool heredoc)
{
	int		i;
	char	*value;
	int		start;
	char **envp;

	envp = ms->env.envp;
	if (!str || !envp)
		return (NULL);
	i = 1;
	start = i;
	if (str[i] && (!ft_isalnum(str[i]) && heredoc == true)) //usar isto so para o heredoc!
	{
		*key = ft_strdup(str);
		return (*key);
	}
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	*key = ft_substr(str, start, i - start);
	if (!*key)
		return (NULL);
	value = ft_get_env(*key, ms);
	return (value);
}

// /**
//  * @brief  Replaces a substring in a string with a new value.
//  * 
//  * @param  value      Pointer to the string to modify.
//  * @param  key        Substring (key) to replace.
//  * @param  ptr        Pointer to the position of the key in the string.
//  * @param  env_value  New value to replace the key with.
//  * @return int        Status of the replacement.
//  **         0 on success.
//  **         1 on error.
//  */
// int	ft_replace_str(char **value, char *key, char *ptr, char *env_value)
// {
// 	char	*new_value;
// 	char	*start;
// 	char	*end;
// 	size_t	new_len;

// 	if (!value || !*value || !key || !ptr || !env_value)
// 		return (1);
// 	if (ft_strlen(*value) == ft_strlen(key) + 1)
// 	{
// 		new_value = ft_strdup(env_value);
// 		free(*value);
// 		*value = new_value;
// 		return (0);
// 	}	
// 	start = ft_substr(*value, 0, ptr - *value);
// 	if (!start)
// 		return (1);
// 	// printf("%lu\n", ft_strlen(key));
// 	end = ft_strdup(ptr + ft_strlen(key) + 1);
// 	if (!end)
// 	{
// 		free(start);
// 		return (1);
// 	}
// 	// printf("%lu\n", ft_strlen(start));
// 	// printf("%lu\n", ft_strlen(env_value));
// 	// printf("%lu\n", ft_strlen(end));
// 	new_len = ft_strlen(start) + ft_strlen(env_value) + ft_strlen(end) + 1;
// 	new_value = malloc(new_len);
// 	if (!new_value)
// 	{
// 		free(start);
// 		free(end);
// 		return (1);
// 	}
// 	ft_strlcpy(new_value, start, new_len);
// 	ft_strlcat(new_value, env_value, new_len);
// 	ft_strlcat(new_value, end, new_len);
// 	free(start);
// 	free(end);
// 	free(*value);
// 	*value = new_value;
// 	return (0);
// }

/**
 * @brief  Replaces a substring in a string with a new value.
 * 
 * @param  value      Pointer to the string to modify.
 * @param  key        Substring (key) to replace.
 * @param  ptr        Pointer to the position of the key in the string.
 * @param  env_value  New value to replace the key with.
 * @return int        Status of the replacement.
 **         0 on success.
 **         1 on error.
 */
int	ft_replace_str(char **value, char *key, char *ptr, char *env_value)
{
	char	*start;
	char	*end;
	char	*new_value;

	if (!value || !*value || !key || !ptr || !env_value)
		return (1);
	if (ft_strlen(*value) == ft_strlen(key) + 1)
		return (ft_replace_entire_str(value, env_value));
	start = ft_substr(*value, 0, ptr - *value);
	end = ft_strdup(ptr + ft_strlen(key) + 1);
	if (!start || !end)
		return (ft_cleanup(start, end, 1));
	new_value = ft_create_replaced_str(start, env_value, end);
	if (!new_value)
		return (1);
	free(*value);
	*value = new_value;
	return (0);
}

/**
 * @brief  Creates a new string replacing the target substring.
 * 
 * @param  start      Start part of the string.
 * @param  env_value  New value to insert.
 * @param  end        End part of the string.
 * @return char*      Newly allocated string with replacement.
 */
static char	*ft_create_replaced_str(char *start, char *env_value, char *end)
{
	char	*new_value;
	size_t	new_len;

	new_len = ft_strlen(start) + ft_strlen(env_value) + ft_strlen(end) + 1;
	new_value = malloc(new_len);
	if (!new_value)
	{
		ft_cleanup(start, end, 1);
		return (NULL);
	}
	ft_strlcpy(new_value, start, new_len);
	ft_strlcat(new_value, env_value, new_len);
	ft_strlcat(new_value, end, new_len);
	ft_cleanup(start, end, 0);
	return (new_value);
}

/**
 * @brief  Cleans up allocated memory in case of failure.
 * 
 * @param  start  Start part of the string.
 * @param  end    End part of the string.
 * @param  error  1 if returning an error, 0 otherwise.
 * @return int    Always returns the error parameter.
 */
int	ft_cleanup(char *to_free1, char *to_free2, int error)
{
	if (to_free1)
		free(to_free1);
	if (to_free2)
		free(to_free2);
	return (error);
}

/**
 * @brief  Replaces the entire string when it's fully equal to the key.
 * 
 * @param  value      Pointer to the string to modify.
 * @param  env_value  New value to replace the key with.
 * @return int        0 on success.
 */
static int	ft_replace_entire_str(char **value, char *env_value)
{
	char	*new_value;

	new_value = ft_strdup(env_value);
	free(*value);
	*value = new_value;
	return (0);
}


/**
 * @brief  Checks if the quotes in a string are balanced up to a specific index.
 * 
 * @param  str  String to check.
 * @param  idx  Index up to which the string should be checked.
 * @return int  Boolean indicating whether the quotes are balanced.
 **         1 if balanced.
 **         0 otherwise.
 */
int	ft_check_balanced_quotes(char *str, int idx)
{
	int		i;
	char	quote_type;
	int		is_balanced;

	i = 0;
	quote_type = '\0';
	is_balanced = 1;
	while (i < idx && str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			if (quote_type == str[i])
				quote_type = '\0'; // Fecha aspas
			else if (quote_type == '\0')
				quote_type = str[i]; // Abre aspas
		}
		i++;
	}
	if (quote_type != '\0')
		is_balanced = 0; // Par de aspas não está fechado
	return (is_balanced);
}

/**
 * @brief  Determines if a variable should be expanded based on its context.
 * 
 * @param  str     String containing the variable.
 * @param  ptr     Pointer to the position of the `$` character in the string.
 * @param  heredoc Flag indicating if the check is within a heredoc.
 * @return int     Expansion behavior.
 **         1 to expand normally.
 **         2 for special case `$?`.
 **         0 if no expansion should occur.
 */
// int	ft_check_if_expand(char *str, char *ptr, int heredoc)
// {
// 	int		i;
// 	char	quote_type;

// 	i = 0;
// 	quote_type = '\0';
// 	while (str[i])
// 	{
// 		if (str[i] == '"' || str[i] == '\'')
// 		{
// 			if (quote_type == str[i])
// 				quote_type = '\0'; // Fecha aspas
// 			else if (quote_type == '\0')
// 				quote_type = str[i]; // Abre aspas
// 			i++;
// 			continue;
// 		}
// 		if (&str[i] == ptr) // Verifica se é o ponteiro para $
// 		{
// 			if (str[i + 1] == '\'' || str[i + 1] == '"')
// 			{
// 				if (ft_check_balanced_quotes(str, i))
// 					return (1); // Expande para string vazia
// 				return (0); // Aspas pertencem a pares separados
// 			}
// 			if (quote_type == '\'' && !heredoc) // Aspas simples: não expande
// 				return (0);
// 			if (str[i + 1] == '?') // Trata caso especial do $?
// 				return (2);
// 			if (!str[i + 1] || str[i + 1] == ' ' || str[i + 1] == '$' ||
// 				str[i + 1] == '.' || str[i + 1] == ',' || str[i + 1] == '!' ||
// 				str[i + 1] == '?' || str[i + 1] == ';' || str[i + 1] == ':' ||
// 				str[i + 1] == '~' || str[i + 1] == '^' || str[i + 1] == '-' ||
// 				str[i + 1] == '+' || str[i + 1] == '*' || str[i + 1] == '/') // Verifica caracteres que invalidam a expansão
// 				return (0); // Retorna 0 para indicar que $ é literal
// 			return (1);
// 		}
// 		i++;
// 	}
// 	return (0); // Retorna 0 se não encontrar $ para expandir
// }

/**
 * @brief  Determines if a `$` should be expanded in a given string.
 * 
 * @param  str      The input string containing `$`.
 * @param  ptr      Pointer to the `$` character in the string.
 * @param  heredoc  Indicates if the function is being called inside a heredoc.
 * @return int      Expansion status.
 **         0 if the `$` should not be expanded.
 **         1 if it should be replaced with an environment variable.
 **         2 if it represents `$?` (exit status).
 */
int	ft_check_if_expand(char *str, char *ptr, int heredoc)
{
	int		i;
	char	quote_type;

	i = 0;
	quote_type = '\0';
	while (str[i])
	{
		if (ft_verify_quotes(&quote_type, str[i]))
		{
			i++;
			continue;
		}
		if (&str[i] == ptr)
			return (ft_check_expansion_conditions(str, i, heredoc, quote_type));
		i++;
	}
	return (0);
}

/**
 * @brief  Handles quote state changes in the input string.
 * 
 * @param  quote_type  Pointer to the current active quote type.
 * @param  c          The current character in the string.
 * @return int        1 if the character is a quote, 0 otherwise.
 */
static int	ft_verify_quotes(char *quote_type, char c)
{
	if (c == '"' || c == '\'')
	{
		if (*quote_type == c)
			*quote_type = '\0'; // Fecha aspas
		else if (*quote_type == '\0')
			*quote_type = c; // Abre aspas
		return (1);
	}
	return (0);
}

/**
 * @brief  Determines whether a `$` should be expanded based on context.
 * 
 * @param  str        The input string.
 * @param  i          Index of the `$` character.
 * @param  heredoc    Whether the function is being called inside a heredoc.
 * @param  quote_type Current active quote type.
 * @return int        Expansion status.
 */
static int	ft_check_expansion_conditions(char *str, int i, int heredoc, char quote_type)
{
	if (str[i + 1] == '\'' || str[i + 1] == '"')
	{
		if (ft_check_balanced_quotes(str, i))
			return (1);
		return (0);
	}
	if (quote_type == '\'' && !heredoc) // Aspas simples: não expande
		return (0);
	if (str[i + 1] == '?') // Trata caso especial do `$?`
		return (2);
	if (ft_is_invalid_dollar_char(str[i + 1]))
		return (0);
	return (1);
}

/**
 * @brief  Checks if the character after `$` is an invalid expansion character.
 * 
 * @param  c  The character after `$`.
 * @return int  1 if the character is invalid for expansion, 0 otherwise.
 */
static int	ft_is_invalid_dollar_char(char c)
{
	return (!c || c == ' ' || c == '$' || c == '.' || c == ',' || c == '!' ||
			c == '?' || c == ';' || c == ':' || c == '~' || c == '^' || c == '-' ||
			c == '+' || c == '*' || c == '/');
}


/**
 * @brief  Replaces variables in heredoc input with their corresponding environment values.
 * 
 * @param  input  Pointer to the heredoc input string.
 * @param  ms     Pointer to the minishell structure.
 * @return int    Status of the operation.
 **         0 on success.
 **         1 on error.
 */
// int	ft_revalue_heredoc_input(char **input, t_minishell *ms)
// {
// 	char	*env_value;
// 	char	*ptr;
// 	char	*key;

// 	env_value = NULL;
// 	key = NULL;
// 	ptr = NULL;
// 	if (!ms || !input || !ms->env.envp)
// 		return (1);
// 	while (input)
// 	{
// 		ptr = ft_strchr(*input, '$');
// 		while (ptr != NULL)
// 		{
// 			if (ft_check_if_expand(*input, ptr, 1) == 1)
// 			{
// 				env_value = ft_get_env_value(ptr, ms, &key);
// 				if (!env_value) 
//  					env_value = ft_strdup("");
// 				ft_replace_str(input, key, ptr, env_value);
// 				if (*key)
// 					free(key);
// 				if (!input)
// 					return (1);
// 				ptr = ft_strchr(*input, '$');
// 				if (!ptr)
// 					break ;
// 			}
// 			else if (ft_check_if_expand(*input, ptr, 1) == 2)
// 			{
// 				key = ft_strdup("?");
// 				ft_replace_str(input, key, ptr, ft_itoa(ft_exit_code(ms)));
// 				free(key);
// 				ptr = ft_strchr(*input, '$');
// 				if (!ptr)
// 					break ;
// 			}
// 			else
// 			{
// 				ptr++;
// 				ptr = ft_strchr(ptr, '$');
// 				if (!ptr)
// 					break ;
// 			}
// 		}
// 		if (!ptr)
// 			return (0);
// 	}
// 	return (0);
// }

/**
 * @brief  Expands environment variables in heredoc input.
 * 
 * @param  input  Pointer to the heredoc input string.
 * @param  ms     Pointer to the minishell structure.
 * @return int    0 on success, 1 on error.
 */
int	ft_revalue_heredoc_input(char **input, t_minishell *ms)
{
	char	*ptr;

	if (!ms || !input || !*input || !ms->env.envp)
		return (1);
	ptr = ft_strchr(*input, '$');
	while ((ptr/* ptr = ft_strchr(*input, '$' */))
	{
		if (ft_process_heredoc_expansion(input, ms, &ptr))
			return (1);
		// if (!ptr)
		// 	break;
	}
	return (0);
}

/**
 * @brief  Handles expansion of a single variable inside heredoc input.
 * 
 * @param  input  Pointer to the heredoc input string.
 * @param  ms     Pointer to the minishell structure.
 * @param  ptr    Pointer to the position of the '$' in the string.
 * @return int    0 on success, 1 on error.
 */
static int	ft_process_heredoc_expansion(char **input, t_minishell *ms, char **ptr)
{
	int	expansion_type;

	expansion_type = ft_check_if_expand(*input, *ptr, 1);
	if (expansion_type == 1)
		ft_expand_env_var(input, ms, ptr);
	else if (expansion_type == 2)
		ft_expand_exit_code(input, ms, ptr);
	if (ptr && *ptr)
		*ptr = ft_strchr((*ptr) + 1, '$');
	if (!ptr)
		return (1);
	return (0);
}

/**
 * @brief  Expands an environment variable in heredoc input.
 * 
 * @param  input  Pointer to the heredoc input string.
 * @param  ms     Pointer to the minishell structure.
 * @param  ptr    Pointer to the position of the '$' in the string.
 * @return int    0 on success, 1 on error.
 */
static int	ft_expand_env_var(char **input, t_minishell *ms, char **ptr)
{
	char	*env_value;
	char	*key;

	env_value = ft_get_env_value(*ptr, ms, &key, true);
	if (!env_value)
		env_value = ft_strdup("");
	if (key != env_value)
	{
		ft_replace_str(input, key, *ptr, env_value);
		*ptr = ft_strchr(*input, '$');
		return (0);
	}	
	if (!*input)
		return (1);
	free(key);
	return (1);
}

/**
 * @brief  Expands the exit code `$?` inside heredoc input.
 * 
 * @param  input  Pointer to the heredoc input string.
 * @param  ms     Pointer to the minishell structure.
 * @param  ptr    Pointer to the position of the '$' in the string.
 */
static void	ft_expand_exit_code(char **input, t_minishell *ms, char **ptr)
{
	char	*key;

	key = ft_strdup("?");
	ft_replace_str(input, key, *ptr, ft_itoa(ft_exit_code(ms)));
	free(key);
	*ptr = ft_strchr(*input, '$');
}


/**
 * @brief  Retrieves the value of an environment variable from the environment array.
 * 
 * @param  key  Variable name to search for.
 * @param  ms   Pointer to the minishell structure.
 * @return char*  Pointer to the value of the variable.
 **         NULL if the variable does not exist.
 */
char	*ft_get_env(const char *key, t_minishell *ms)
{
	int		i;
	size_t	len;
	char	**envp;

	envp = ms->env.envp;
	len = ft_strlen(key);
	i = 0;
	if (!key || !envp)
		return (NULL);
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], key, len) && envp[i][len] == '=')
			return (&envp[i][len + 1]);
		i++;
	}
	return (NULL);
}

/**
 * @brief  Sets or updates an environment variable in the environment array.
 * 
 * @param  key    Variable name to set or update.
 * @param  value  Value to assign to the variable.
 * @param  ms     Pointer to the minishell structure.
 * @return int    Status of the operation.
 **         0 on success.
 **         1 on error.
 */
int	ft_set_env(const char *key, const char *value, t_minishell *ms)
{
	int		i;
	size_t	len;
	char	*new_var;

	i = 0;
	len = ft_strlen(key);
	new_var = ft_strjoin(ft_strjoin(key, "="), value);
	if (!key || !value || !ms || !ms->env.envp || !new_var)
		return (1);
	while (ms->env.envp[i])
	{
		if (!ft_strncmp(ms->env.envp[i], key, len) && ms->env.envp[i][len] == '=')
		{
			free(ms->env.envp[i]);
			ms->env.envp[i] = new_var;
			return (0);
		}
		i++;
	}
	ms->env.envp[i] = new_var;
	ms->env.envp[i + 1] = NULL;
	return (0);
}

/**
 * @brief  Unsets (removes) an environment variable from the environment array.
 * 
 * @param  key  Variable name to remove.
 * @param  ms   Pointer to the minishell structure.
 * @return int  Status of the operation.
 **         0 on success.
 **         1 if the variable does not exist or on error.
 */
int ft_unset_env(const char *key, t_minishell *ms)
{
	int i;
	int j;
	size_t len;

	len = ft_strlen(key);
	i = 0;
	j = 0;
	if (!key || !ms || !ms->env.envp)
		return (1);
	while (ms->env.envp[i])
	{
		if (!ft_strncmp(ms->env.envp[i], key, len) && ms->env.envp[i][len] == '=')
		{
			free(ms->env.envp[i]);
			j = i;
			while (ms->env.envp[j])
			{
				ms->env.envp[j] = ms->env.envp[j + 1];
				j++;
			}
			return (0);
		}
		i++;
	}
	return (1);
}
