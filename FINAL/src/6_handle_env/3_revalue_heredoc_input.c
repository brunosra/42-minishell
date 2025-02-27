/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_revalue_heredoc_input.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 05:13:41 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/02/27 02:40:15 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int			ft_revalue_heredoc_input(char **input, t_minishell *ms);
static int	ft_process_heredoc_expansion(char **input, t_minishell *ms,
				char **ptr);
static int	ft_expand_env_var(char **input, t_minishell *ms, char **ptr);
static void	ft_expand_exit_code(char **input, t_minishell *ms, char **ptr);

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
	while ((ptr))
	{
		if (ft_process_heredoc_expansion(input, ms, &ptr))
			return (1);
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
static int	ft_process_heredoc_expansion(char **input, t_minishell *ms,
											char **ptr)
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
