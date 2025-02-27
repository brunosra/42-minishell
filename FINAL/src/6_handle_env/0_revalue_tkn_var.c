/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_revalue_tkn_var.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 05:03:13 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/02/27 03:55:56 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int			ft_revalue_tkn_var(t_minishell *ms);
static char	*ft_process_token_expansion(t_minishell *ms, t_token *token,
				char *ptr);
static int	ft_is_expandable_token(t_type type);

/**
 * @brief  Expands environment variables in tkns of type VAR, CMD, FILE, or ARG.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return int Status code.
 **         0 on success.
 **         1 on error.
 */
int	ft_revalue_tkn_var(t_minishell *ms)
{
	int		i;
	char	*ptr;

	if (!ms || !ms->tokens || !ms->env.envp)
		return (1);
	i = -1;
	while (ms->tokens[++i].value)
	{
		if (ft_is_expandable_token(ms->tokens[i].type) && i > 0
			&& ms->tokens[i - 1].type != TKN_HDOC)
		{
			ptr = ft_strchr(ms->tokens[i].value, '$');
			while (ptr && *ptr)
				ptr = ft_process_token_expansion(ms, &ms->tokens[i], ptr);
		}
	}
	return (0);
}

/**
 * @brief  Expands a `$` variable inside a token.
 * 
 * @param  ms     Pointer to the minishell structure.
 * @param  token  Pointer to the token being processed.
 * @param  ptr    Pointer to the `$` inside the token value.
 * @return char*  Pointer to the next `$` to expand, or NULL if none remain.
 */
static char	*ft_process_token_expansion(t_minishell *ms, t_token *token,
				char *ptr)
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
 * @brief  Checks if a token type is eligible for expansion.
 * 
 * @param  type  Token type.
 * @return int   1 if expandable, 0 otherwise.
 */
static int	ft_is_expandable_token(t_type type)
{
	return (type == TKN_VAR || type == TKN_CMD || type == TKN_FILE
		|| type == TKN_ARG);
}
