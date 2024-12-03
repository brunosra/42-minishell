/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hendle_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:50:15 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/12/03 05:13:47 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		ft_revalue_token_variable(t_minishell *ms);
int		ft_check_balanced_quotes(char *str, int idx);
char	*ft_get_env(const char *key, char **envp);
char	**ft_duplicate_envp(char **envp);
int		ft_check_if_expand(char *str, char *ptr);
int		ft_replace_str(char **value, char *key, char *ptr, char *env_value);
char	*ft_get_env_value(const char *str, char **envp, char **key);
/* int		ft_remove_str(char **value, char *key, char *ptr);
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
		if (ms->tokens[i].type == TOKEN_VARIABLE || ms->tokens[i].type == TOKEN_COMMAND)
		{
			ptr = ft_strchr(ms->tokens[i].value, '$');
			while (ptr != NULL)
			{
				if (ft_check_if_expand(ms->tokens[i].value, ptr) == 1)
				{
					env_value = ft_get_env_value(ptr, ms->env.envp, &key);
					if (!env_value) // ALterar isto!!!
					{
/* 						ft_putstr_fd("Erro: Variável '", 2); // ALterar isto!!!
						ft_putstr_fd(ms->tokens[i].value, 2); // ALterar isto!!!
						ft_putstr_fd("' não encontrada\n", 2); // ALterar isto!!!
 */						env_value = ft_strdup("");
					}
/* 					if (*env_value == '\0' && ft_strlen(ms->tokens[i].value) == ft_strlen(key + 1))
						ft_remove_str(&ms->tokens[i].value, key, ptr);
					// free(ms->tokens[i].value);
					else */
					/* ms->tokens[i].value =  */ft_replace_str(&ms->tokens[i].value, key, ptr, env_value);
					if (*key)
						free(key);
					if (!ms->tokens[i].value)
					{
						perror("");
						return (1);
					}
					if (ms->tokens[i].value[0] == '\0' && ms->tokens[i].type == TOKEN_COMMAND)
						ms->tokens[i].type = TOKEN_VARIABLE;
					ptr = ft_strchr(ms->tokens[i].value, '$');
					if (!ptr)
						break ;
				}
				else if (ft_check_if_expand(ms->tokens[i].value, ptr) == 2)
				{
					key = ft_strdup("?");
					// printf("%lu\n", ft_strlen(ft_itoa(ms->exit_code)));
					ft_replace_str(&ms->tokens[i].value, key, ptr, ft_itoa(ms->exit_code));
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

char	*ft_get_env_value(const char *str, char **envp, char **key)
{
	int		i;
	char	*value;
	int		start;

	if (!str || !envp)
		return (NULL);
	i = 1;
	start = i;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
/* 	if (str[i] != ' ')
		*key = ft_substr(str, start, i - 1 - start);
	else */
		*key = ft_substr(str, start, i - start);
	if (!*key)
		return (NULL);
	value = ft_get_env(*key, envp);
	return (value);
}

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

char	*ft_get_env(const char *key, char **envp)
{
	int		i;
	size_t	len;

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


int	ft_check_if_expand(char *str, char *ptr)
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
			if (str[i + 1] == '?') // Trata caso especial do $?
				return (2); // Retorna 2 para indicar $? (exit status)

			// Verifica se há aspas consecutivas logo após $
			if (str[i + 1] == '\'' || str[i + 1] == '"')
			/* if ((str[i + 1] == '"' && str[i + 2] == '"') ||
				(str[i + 1] == '\'' && str[i + 2] == '\'') || (str[i + 1] == '"' && str[i + 2] == '\'') || (str[i + 1] == '\'' && str[i + 2] == '"'))
			 */{
				// Verifica se essas aspas pertencem ao mesmo par
				if (ft_check_balanced_quotes(str, i))
					return (1); // Expande para string vazia
				return (0); // Aspas pertencem a pares separados
			}

			// Aspas simples: não expande
			if (quote_type == '\'')
				return (0);

			// Verifica caracteres que invalidam a expansão
			if (!str[i + 1] || str[i + 1] == ' ' || str[i + 1] == '$' ||
				str[i + 1] == '.' || str[i + 1] == ',' || str[i + 1] == '!' ||
				str[i + 1] == '?' || str[i + 1] == ';' || str[i + 1] == ':' ||
				str[i + 1] == '~' || str[i + 1] == '^' || str[i + 1] == '-' ||
				str[i + 1] == '+' || str[i + 1] == '*' || str[i + 1] == '/')
				return (0); // Retorna 0 para indicar que $ é literal

			// Caso contrário, expande
			return (1);
		}

		i++;
	}

	return (0); // Retorna 0 se não encontrar $ para expandir
}




/* int	ft_remove_str(char **value, char *key, char *ptr)
{
	char	*new_value;
	char	*start;
	char	*end;
	size_t	new_len;

	if (!value || !*value || !key || !ptr)
		return (1);
	start = ft_substr(*value, 0, ptr - *value);
	if (!start)
		return (1);
	end = ft_strdup(++ptr + ft_strlen(key) + 1);
	if (!end)
	{
		free(start);
		return (1);
	}
	new_len = ft_strlen(start) + ft_strlen(end) + 1;
	new_value = malloc(new_len);
	if (!new_value)
	{
		free(start);
		free(end);
		return (1);
	}
	ft_strlcpy(new_value, start, new_len);
	ft_strlcat(new_value, end, new_len);
	free(start);
	free(end);
	free(*value);
	*value = new_value;
	return (0);
}
 */