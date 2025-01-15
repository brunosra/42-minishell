/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:50:15 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/01/15 04:47:36 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
extern volatile sig_atomic_t g_interrupt;

int		ft_revalue_token_variable(t_minishell *ms);
int		ft_check_balanced_quotes(char *str, int idx);
char	**ft_duplicate_envp(char **envp);
int		ft_check_if_expand(char *str, char *ptr, int heredoc);
int		ft_replace_str(char **value, char *key, char *ptr, char *env_value);
int		ft_revalue_heredock_input(char **input, t_minishell *ms);
char	*ft_get_env_value(const char *str, t_minishell *ms, char **key);
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
int	ft_revalue_token_variable(t_minishell *ms)
{
	int		i;
	char	*env_value;
	char	*ptr;
	char	*key;

	i = -1;
	env_value = NULL;
	key = NULL;
	ptr = NULL;
	if (!ms || !ms->tokens || !ms->env.envp)
		return (1);
	while (ms->tokens[++i].value)
	{
		// ft_putstr_fd(ms->tokens[i].value, 1);
		if (ms->tokens[i].type == TOKEN_VARIABLE || ms->tokens[i].type == TOKEN_COMMAND || ms->tokens[i].type == TOKEN_FILENAME || ms->tokens[i].type == TOKEN_ARGUMENT)
		{
			ptr = ft_strchr(ms->tokens[i].value, '$');
			while (ptr != NULL)
			{
				if (ft_check_if_expand(ms->tokens[i].value, ptr, 0) == 1)
				{
					env_value = ft_get_env_value(ptr, ms, &key);
					if (!env_value) 
 						env_value = ft_strdup("");
					ft_replace_str(&ms->tokens[i].value, key, ptr, env_value);
					if (*key)
						free(key);
					if (!ms->tokens[i].value)
					{
						perror("");
						return (1);
					}
					if (ms->tokens[i].value[0] == '\0' && ms->tokens[i].type == TOKEN_COMMAND)
						ms->tokens[i].type = TOKEN_VARIABLE; // alaterar para TOKEN_EMPTY para ser ignorado no parsing. outra e perceber a quando do tokenixe type se a expansao da var e nula e se assim for passa a TOKEN_EMPTY...
					ptr = ft_strchr(ms->tokens[i].value, '$');
					if (!ptr)
						break ;
				}
				else if (ft_check_if_expand(ms->tokens[i].value, ptr, 0) == 2)
				{
					key = ft_strdup("?");
					// printf("%lu\n", ft_strlen(ft_itoa(ft_exit_code(ms))));
					ft_replace_str(&ms->tokens[i].value, key, ptr, ft_itoa(ft_exit_code(ms)));
					free(key);
					ptr = ft_strchr(ms->tokens[i].value, '$');
					if (!ptr)
						break ;
				}
				else
				{
					ptr++;
					ptr = ft_strchr(ptr, '$');
					if (!ptr)
						break ;
				}
			}
		}
	}
	return (0);
}

/**
 * @brief  Extracts the value of an environment variable from a given string.
 * 
 * @param  str  String containing the variable name (e.g., `$VAR`).
 * @param  ms   Pointer to the minishell structure.
 * @param  key  Pointer to store the extracted variable name.
 * @return char*  Value of the environment variable.
 **         NULL if the variable does not exist.
 */
char	*ft_get_env_value(const char *str, t_minishell *ms, char **key)
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
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	*key = ft_substr(str, start, i - start);
	if (!*key)
		return (NULL);
	value = ft_get_env(*key, ms);
	return (value);
}

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
	char	*new_value;
	char	*start;
	char	*end;
	size_t	new_len;

	if (!value || !*value || !key || !ptr || !env_value)
		return (1);
	if (ft_strlen(*value) == ft_strlen(key) + 1)
	{
		new_value = ft_strdup(env_value);
		free(*value);
		*value = new_value;
		return (0);
	}	
	start = ft_substr(*value, 0, ptr - *value);
	if (!start)
		return (1);
	// printf("%lu\n", ft_strlen(key));
	end = ft_strdup(ptr + ft_strlen(key) + 1);
	if (!end)
	{
		free(start);
		return (1);
	}
	// printf("%lu\n", ft_strlen(start));
	// printf("%lu\n", ft_strlen(env_value));
	// printf("%lu\n", ft_strlen(end));
	new_len = ft_strlen(start) + ft_strlen(env_value) + ft_strlen(end) + 1;
	new_value = malloc(new_len);
	if (!new_value)
	{
		free(start);
		free(end);
		return (1);
	}
	ft_strlcpy(new_value, start, new_len);
	ft_strlcat(new_value, env_value, new_len);
	ft_strlcat(new_value, end, new_len);
	free(start);
	free(end);
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
int	ft_check_if_expand(char *str, char *ptr, int heredoc)
{
	int		i;
	char	quote_type;

	i = 0;
	quote_type = '\0';
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			if (quote_type == str[i])
				quote_type = '\0'; // Fecha aspas
			else if (quote_type == '\0')
				quote_type = str[i]; // Abre aspas
			i++;
			continue;
		}
		if (&str[i] == ptr) // Verifica se é o ponteiro para $
		{
			if (str[i + 1] == '\'' || str[i + 1] == '"')
			{
				if (ft_check_balanced_quotes(str, i))
					return (1); // Expande para string vazia
				return (0); // Aspas pertencem a pares separados
			}
			if (quote_type == '\'' && !heredoc) // Aspas simples: não expande
				return (0);
			if (str[i + 1] == '?') // Trata caso especial do $?
				return (2);
			if (!str[i + 1] || str[i + 1] == ' ' || str[i + 1] == '$' ||
				str[i + 1] == '.' || str[i + 1] == ',' || str[i + 1] == '!' ||
				str[i + 1] == '?' || str[i + 1] == ';' || str[i + 1] == ':' ||
				str[i + 1] == '~' || str[i + 1] == '^' || str[i + 1] == '-' ||
				str[i + 1] == '+' || str[i + 1] == '*' || str[i + 1] == '/') // Verifica caracteres que invalidam a expansão
				return (0); // Retorna 0 para indicar que $ é literal
			return (1);
		}
		i++;
	}
	return (0); // Retorna 0 se não encontrar $ para expandir
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
int	ft_revalue_heredock_input(char **input, t_minishell *ms)
{
	char	*env_value;
	char	*ptr;
	char	*key;

	env_value = NULL;
	key = NULL;
	ptr = NULL;
	if (!ms || !input || !ms->env.envp)
		return (1);
	while (input)
	{
		ptr = ft_strchr(*input, '$');
		while (ptr != NULL)
		{
			if (ft_check_if_expand(*input, ptr, 1) == 1)
			{
				env_value = ft_get_env_value(ptr, ms, &key);
				if (!env_value) 
 					env_value = ft_strdup("");
				ft_replace_str(input, key, ptr, env_value);
				if (*key)
					free(key);
				if (!input)
				{
					perror("");
					return (1);
				}
				ptr = ft_strchr(*input, '$');
				if (!ptr)
					break ;
			}
			else if (ft_check_if_expand(*input, ptr, 1) == 2)
			{
				key = ft_strdup("?");
				// printf("%lu\n", ft_strlen(ft_itoa(ft_exit_code(ms))));
				ft_replace_str(input, key, ptr, ft_itoa(ft_exit_code(ms)));
				free(key);
				ptr = ft_strchr(*input, '$');
				if (!ptr)
					break ;
			}
			else
			{
				ptr++;
				ptr = ft_strchr(ptr, '$');
				if (!ptr)
					break ;
			}
		}
		if (!ptr)
			return (0);
	}
	return (0);
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
	if (!key || !value || !ms || !ms->env.envp)
		return (1);
	if (!new_var)
	{
		perror("ft_set_env: ft_strjoin");
		return (1);
	}
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