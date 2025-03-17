/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_process_input_and_execute.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 00:04:02 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/03/17 18:59:33 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int			ft_process_input_and_execute(t_minishell *ms);
static int	ft_find_tkn_excpt(t_token *tokens);
static void	ft_check_files(t_token *tokens);
static int	ft_handle_and_tokenize_input(t_minishell *ms);

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
	if (ft_find_tkn_excpt(ms->tokens))
	{
		ft_free_tokens(ms->tokens);
		return (1);
	}
	ft_trim_last_cat_sequence(ms);
	ms->ast_root = ft_parse_ast(ms->tokens);
	if (ms->ast_root)
		ft_finalize_execution(ms);
	return (0);
}

static int	ft_find_tkn_excpt(t_token *tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return (1);
	while (tokens[i].type != TKN_NULL)
	{
		if (tokens[i].type == TKN_EXCPT)
		{
			ft_putstr_three_fd("minishell: syntax error near unexpected token"
				" `", tokens[i].value, "'\n", STDERR_FILENO);
			ft_exit_code(2);
			return (1);
		}
		i++;
	}
	if (i > 2 && tokens[i].type == TKN_NULL && tokens[i - 1].type == TKN_FILE
		&& (tokens[i - 2].type == TKN_IN_RD || tokens[i - 2].type == TKN_OUT_RD)
		&& tokens[i - 3].type == TKN_PIPE)
	{
		ft_check_files(tokens);
		return (1);
	}
	return (0);
}

/**
 * @brief  Checks if files exist for redirection and handles missing files.
 * 
 * This function iterates through the token array and checks if a `TKN_FILE`
 * token is preceded by a `TKN_IN_RD` (input redirection `<`) or `TKN_OUT_RD`
 * (output redirection `>` or `>>`). If the file does not exist:
 *   - Prints an error message.
 *   - Creates the file if it was an output redirection.
 * 
 * @param  tokens  Pointer to the array of tokens.
 * @return void
 */
static void	ft_check_files(t_token *tokens)
{
	int	i;
	int	fd;

	i = 0;
	while (tokens[i].type != TKN_NULL)
	{
		if (tokens[i].type == TKN_FILE && i > 0)
		{
			if (tokens[i - 1].type == TKN_IN_RD
				&& access(tokens[i].value, F_OK) == -1)
			{
				ft_putstr_three_fd("minishell: ", tokens[i].value,
					": No such file or directory\n", STDERR_FILENO);
			}
			else if (tokens[i - 1].type == TKN_OUT_RD
				&& access(tokens[i].value, F_OK) == -1)
			{
				ft_putstr_three_fd("minishell: ", tokens[i].value,
					": No such file or directory\n", STDERR_FILENO);
				fd = open(tokens[i].value, O_CREAT | O_WRONLY, 0644);
				close(fd);
			}
		}
		i++;
	}
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
