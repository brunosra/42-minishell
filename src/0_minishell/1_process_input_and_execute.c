/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_process_input_and_execute.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 00:04:02 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/03/07 08:08:36 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int			ft_process_input_and_execute(t_minishell *ms);
static int	ft_handle_and_tokenize_input(t_minishell *ms);
static void	ft_find_stuck_cats(t_minishell *ms, t_node *node);

/**
 * @brief  Process the user input and execute commands.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return int
 **        0 on successful execution
 **        1 on error
 */
int	ft_process_input_and_execute(t_minishell *ms)
{
	if (ft_handle_and_tokenize_input(ms))
	{
		if (ft_exit_code(-1) == 2)
			return (1);
		else
			return (ft_putstr_and_return("minishell: unclosed quotes\n", 1));
	}
	if (ft_is_cat_pipeline(ms->tokens))
		ms->tokens = ft_trim_tokens_before_pipe(ms->tokens, ms);
	ms->ast_root = ft_parse_ast(ms->tokens);
	if (ms->in_pipe == false)
		ft_find_stuck_cats(ms, ms->ast_root);
	if (ms->ast_root)
		ft_finalize_execution(ms);
	return (0);
}

/**
 * @brief  Tokenize the user input and check for unclosed quotes.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return int
 **        0 on success
 **        1 if there are unclosed quotes or other errors
 */
static int	ft_handle_and_tokenize_input(t_minishell *ms)
{
	if (ft_check_quotes(ms->input))
		return (1);
	ms->n_args = ft_count_args(ms->input, 0);
	if (ms->n_args == -1)
		return (1);
	ms->tokens = ft_tokenize_input(ms->input, ms->n_args, 0, 0);
	if (ms->tokens == NULL)
		return (1);
	ft_revalue_tkn_var(ms);
	ms->tokens = ft_handle_empty_tokens(ms->tokens);
	return (0);
}

/**
 * @brief  Find and count stuck "cat" commands in the AST.
 * 
 * @param  ms    Pointer to the minishell structure.
 * @param  node  Current node in the AST.
 * @return void
 */
static void	ft_find_stuck_cats(t_minishell *ms, t_node *node)
{
	t_node	*current;

	current = node;
	if (!current)
		return ;
	if (current->token->type == TKN_CMD && current->token->value[0] != '\0')
	{
		if (current->cmd_ready[1] == NULL
			&& (!ft_strcmp(current->cmd_ready[0], "cat")
				|| !ft_strcmp(current->cmd_ready[0], "/bin/cat"))
			&& current->prev && current->prev->token->type == TKN_PIPE
			&& (current->prev->left == current
				|| (current->prev->prev
					&& current->prev->prev->token->type == TKN_PIPE
					&& current->prev->right == current)))
			ms->c_stuck_cats++;
	}
	if (!current->left && !current->right)
		return ;
	ft_find_stuck_cats(ms, current->left);
	if (!ms->c_stuck_cats)
		return ;
	ft_find_stuck_cats(ms, current->right);
}
