/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hendle_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:50:15 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/12/11 19:34:15 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		ft_revalue_token_variable(t_minishell *ms);
int		ft_check_balanced_quotes(char *str, int idx);
char	*ft_get_env(const char *key, char **envp);
char	**ft_duplicate_envp(char **envp);
int	ft_check_if_expand(char *str, char *ptr, int heredoc);
int		ft_replace_str(char **value, char *key, char *ptr, char *env_value);
char	*ft_get_env_value(const char *str, char **envp, char **key);
int	ft_revalue_heredock_input(char **input, t_minishell *ms);


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
		// ft_putstr_fd(ms->tokens[i].value, 1);
		if (ms->tokens[i].type == TOKEN_VARIABLE || ms->tokens[i].type == TOKEN_COMMAND || ms->tokens[i].type == TOKEN_FILENAME || ms->tokens[i].type == TOKEN_ARGUMENT)
		{
			ptr = ft_strchr(ms->tokens[i].value, '$');
			while (ptr != NULL)
			{
				if (ft_check_if_expand(ms->tokens[i].value, ptr, 0) == 1)
				{
					env_value = ft_get_env_value(ptr, ms->env.envp, &key);
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
				env_value = ft_get_env_value(ptr, ms->env.envp, &key);
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
				// printf("%lu\n", ft_strlen(ft_itoa(ms->exit_code)));
				ft_replace_str(input, key, ptr, ft_itoa(ms->exit_code));
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

