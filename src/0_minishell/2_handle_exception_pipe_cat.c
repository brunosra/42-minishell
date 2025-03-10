/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_handle_exception_pipe_cat.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 05:21:58 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/03/10 09:40:23 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void			ft_trim_last_cat_sequence(t_minishell *ms);
static int		ft_find_last_cat(t_token *tokens);
static int		ft_find_first_cat_sequence(t_token *tokens, int last_cat,
					int *seq_count);
static t_token	*ft_copy_tokens_excluding_range(t_minishell *ms,
					t_token *tokens, int start, int end);

/**
 * @brief  Finds the last occurrence of "cat" in the token list.
 * 
 * Iterates over the token list and returns the index of the last "cat"
 * command found.
 * 
 * @param  tokens  Pointer to the array of tokens.
 * @return int     The index of the last "cat" found, or -1 if none exists.
 */
static int	ft_find_last_cat(t_token *tokens)
{
	int	i;
	int	last_cat;

	i = 0;
	last_cat = -1;
	while (tokens[i].type != TKN_NULL)
	{
		if (tokens[i].type == TKN_CMD && !ft_strcmp(tokens[i].value, "cat"))
			last_cat = i;
		i++;
	}
	return (last_cat);
}

/**
 * @brief  Finds the first occurrence of a "cat | cat | ..." sequence.
 * 
 * Traverses the tokens backward from the last "cat", counting how many
 * "cat |" sequences exist in a row.
 * 
 * @param  tokens     Pointer to the array of tokens.
 * @param  last_cat   The index of the last "cat" found.
 * @param  seq_count  Pointer to store the count of "cat |" sequences.
 * @return int        The index of the first "cat" in the sequence, or -1.
 */
static int	ft_find_first_cat_sequence(t_token *tokens, int last_cat,
				int *seq_count)
{
	int	i;
	int	first_cat;

	i = last_cat;
	first_cat = -1;
	*seq_count = 0;
	while (i >= 0)
	{
		if (tokens[i].type == TKN_CMD && !ft_strcmp(tokens[i].value, "cat"))
			i--;
		if (i < 0)
			break ;
		if (tokens[i].type == TKN_PIPE)
		{
			(*seq_count)++;
			i--;
		}
		if (tokens[i].type != TKN_PIPE && (tokens[i].type != TKN_CMD
				|| ft_strcmp(tokens[i].value, "cat")))
			break ;
	}
	first_cat = i + 2;
	return (first_cat);
}

/**
 * @brief  Removes redundant trailing "cat | cat | cat" sequences.
 * 
 * Ensures that if multiple consecutive "cat" commands exist, only one 
 * remains at the end of the pipeline.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return void
 */
void	ft_trim_last_cat_sequence(t_minishell *ms)
{
	int	last_cat;
	int	first_cat;
	int	seq_count;

	last_cat = ft_find_last_cat(ms->tokens);
	if (last_cat == -1)
		return ;
	first_cat = ft_find_first_cat_sequence(ms->tokens, last_cat, &seq_count);
	if (ms->tokens[last_cat + 1].type != TKN_NULL && first_cat == 1) 
	{
		if (ms->in_pipe == false)
			ms->c_stuck_cats = last_cat - first_cat - 2;
	}
	else if (first_cat == 1 && seq_count > 1 && ms->tokens[last_cat + 1].type == TKN_NULL)
	{
		if (ms->in_pipe == false)
			ms->c_stuck_cats = -1;
	}
	else if (last_cat - first_cat >= 1)
	{
		if (ms->in_pipe == false)
			ms->c_stuck_cats = 2;
	}
	if (seq_count > 1 && first_cat > 1)
		ms->tokens = ft_copy_tokens_excluding_range(ms, ms->tokens,
				first_cat, last_cat);
}

/**
 * @brief  Creates a new token array excluding a specified range of tokens.
 * 
 * Allocates a new token list, copying all tokens except those between
 * the given `start` and `end` indices.
 * 
 * @param  ms      Pointer to the minishell structure.
 * @param  tokens  The original array of tokens.
 * @param  start   The index of the first token to exclude.
 * @param  end     The index of the last token to exclude.
 * @return t_token* New token array without the excluded range.
 */
static t_token	*ft_copy_tokens_excluding_range(t_minishell *ms,
					t_token *tokens, int start, int end)
{
	int		i;
	int		j;
	t_token	*new_tokens;

	new_tokens = ft_calloc(sizeof(t_token), ms->n_args - (end - start) + 1);
	if (!new_tokens)
		return (NULL);
	i = -1;
	j = 0;
	while (tokens[++i].type != TKN_NULL)
	{
		if (i <= start || i > end)
		{
			new_tokens[j].value = ft_strdup(tokens[i].value);
			new_tokens[j].type = tokens[i].type;
			if (tokens[i].old_value)
				new_tokens[j].old_value = ft_strdup(tokens[i].old_value);
			j++;
		}
	}
	new_tokens[j].type = TKN_NULL;
	new_tokens[j].value = NULL;
	ms->n_args = j;
	ft_free_tokens(tokens);
	return (new_tokens);
}
