/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_handle_exception_pipe_cat.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 08:06:42 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/03/08 18:26:59 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int			ft_is_cat_pipeline(t_token *tokens);
t_token		*ft_trim_tokens_before_pipe(t_token *tokens, t_minishell *ms);
static int	ft_check_cat_sequence(t_token *tokens, int i);
static int	ft_skip_initial_args(t_token *tokens);

/**
 * @brief Determines if the command sequence is a "cat pipeline".
 * 
 * A valid "cat pipeline" starts with any command, followed by a pipe (`|`),
 * and then only `cat` commands separated by pipes.
 * 
 * @param tokens The array of tokens.
 * @return 1 if it is a valid "cat pipeline", 0 otherwise.
 */
int	ft_is_cat_pipeline(t_token *tokens)
{
	int	i;

	if (!tokens || (!ft_strcmp(tokens[0].value, "cat")
			&& tokens[0].type == TKN_CMD))
		return (0);
	i = ft_skip_initial_args(tokens);
	if (tokens && tokens[0].type != TKN_NULL && tokens[i].type == TKN_PIPE)
		return (ft_check_cat_sequence(tokens, i + 1));
	return (0);
}

/**
 * @brief  Creates a new token array up to the first `|` and updates ms->n_args.
 *
 * @param  tokens  Pointer to the array of tokens.
 * @param  ms      Pointer to the minishell structure.
 * @return t_token* New token array containing only the first segment.
 */
t_token	*ft_trim_tokens_before_pipe(t_token *tokens, t_minishell *ms)
{
	t_token	*new_tokens;
	int		count;
	int		i;

	i = 0;
	count = 0;
	while (tokens[count].type != TKN_PIPE && tokens[count].type != TKN_NULL)
		count++;
	new_tokens = ft_calloc(sizeof(t_token), (count + 1));
	if (!new_tokens)
		return (NULL);
	while (i < count)
	{
		new_tokens[i].value = ft_strdup(tokens[i].value);
		new_tokens[i].type = tokens[i].type;
		if (tokens[i].old_value)
			new_tokens[i].old_value = ft_strdup(tokens[i].old_value);
		i++;
	}
	new_tokens[count].type = TKN_NULL;
	new_tokens[count].value = NULL;
	ft_free_tokens(tokens);
	ms->n_args = count;
	return (new_tokens);
}

/**
 * @brief Checks if a valid "cat pipeline" sequence exists.
 * 
 * This function verifies that after an initial command, the sequence follows
 * the pattern `cat | cat | cat ...` until the end.
 * 
 * @param tokens The array of tokens.
 * @param i The index where the sequence should start.
 * @return 1 if the sequence is valid, 0 otherwise.
 */
static int	ft_check_cat_sequence(t_token *tokens, int i)
{
	int	count;

	count = 0;
	while (tokens[i].type != TKN_NULL)
	{
		if (tokens[i].type == TKN_CMD && !ft_strcmp(tokens[i].value, "cat"))
		{
			i++;
			count++;
		}
		if (tokens[i].type != TKN_PIPE || tokens[i].type == TKN_NULL)
			break ;
		i++;
	}
	if (tokens[i].type == TKN_NULL && !ft_strcmp(tokens[i - 1].value, "cat")
		&& count > 1)
		return (1);
	return (0);
}

/**
 * @brief Skips initial arguments before checking pipeline validity.
 * 
 * This function skips over tokens that are arguments, variables, files,
 * or empty values before reaching the first command.
 * 
 * @param tokens The array of tokens to analyze.
 * @return Index of the first non-argument token.
 */
static int	ft_skip_initial_args(t_token *tokens)
{
	int	i;

	i = 1;
	if (tokens[0].type != TKN_NULL)
	{
		while (tokens[0].type != TKN_NULL && tokens[i].type != TKN_NULL
			&& (tokens[i].type == TKN_ARG
				|| tokens[i].type == TKN_VAR || tokens[i].type == TKN_FILE
				|| tokens[i].type == TKN_EPTY))
			i++;
	}
	return (i);
}
