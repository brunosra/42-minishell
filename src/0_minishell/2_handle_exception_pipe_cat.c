/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_handle_exception_pipe_cat.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 05:21:58 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/03/10 22:23:01 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void			ft_trim_last_cat_sequence(t_minishell *ms);
static int		ft_find_last_cat(t_token *tokens);
static int		ft_find_first_cat_sequence(t_token *tokens, int last_cat,
					int *seq_count);
static t_token	*ft_copy_tokens_excluding_range(t_minishell *ms,
					t_token *tokens, int start, int end);
static int		ft_count_initial_cat_sequence(t_token *tokens);

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
 * @brief  Trims redundant trailing "cat | cat | ..." sequences.
 * 
 * This function identifies and removes extra trailing "cat |" sequences
 * while ensuring that an initial "cat |" sequence is correctly handled.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return void
 */
void	ft_trim_last_cat_sequence(t_minishell *ms)
{
	int	last_cat;
	int	first_cat;
	int	seq_count;
	int	init_cat_seq;

	last_cat = ft_find_last_cat(ms->tokens);
	if (last_cat == -1)
		return ;
	first_cat = ft_find_first_cat_sequence(ms->tokens, last_cat, &seq_count);
	init_cat_seq = ft_count_initial_cat_sequence(ms->tokens);
	if (init_cat_seq > 0 && ms->in_pipe == false)
		ms->c_stuck_cats = init_cat_seq;
	else if (init_cat_seq == -1 && ms->in_pipe == false)
		ms->c_stuck_cats = -1;
	else if (last_cat - first_cat >= 1 && ms->in_pipe == false)
		ms->c_stuck_cats = 0;
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

/**
 * @brief  Counts the initial "cat | cat | ... cat" sequence.
 * 
 * This function ensures that the token array starts with "cat | cat | ..." 
 * and determines whether it ends in another command or remains a 
 * "cat-only pipeline".
 * 
 * @param  tokens  Pointer to the array of tokens.
 * @return int     
 *         - `-1` if the sequence consists only of "cat | cat | ... cat".
 *         - `count` if it ends in another command.
 *         - `0` if the sequence does not start with "cat |".
 */
static int	ft_count_initial_cat_sequence(t_token *tokens)
{
	int	i;
	int	count;

	i = 1;
	if (!tokens || tokens[0].type == TKN_NULL
		|| !(tokens[0].type == TKN_CMD && !ft_strcmp(tokens[0].value, "cat")))
		return (0);
	count = 1;
	while (tokens[i].type != TKN_NULL)
	{
		if (tokens[i].type == TKN_PIPE)
		{
			i++;
			if (tokens[i].type == TKN_CMD && !ft_strcmp(tokens[i].value, "cat"))
				count++;
			else
				return (count);
		}
		else
			return (0);
		i++;
	}
	return (-1);
}
