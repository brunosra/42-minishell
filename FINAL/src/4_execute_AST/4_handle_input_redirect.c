/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   4_handle_input_redirect.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 02:32:06 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/02/27 00:54:38 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int			ft_handle_input_redirect(t_node *node, t_minishell *ms);
static int	ft_validate_input_file(t_node *node, t_minishell *ms);
int			ft_invalid_right_token_value(char *value);
void		ft_swap_redirects_values(t_node *node, t_type type);

/**
 * @brief  Handles input redirection from a file.
 * 
 * @param  node  Pointer to the input redirection node in the AST.
 * @param  ms    Pointer to the minishell structure.
 * @return int   Execution status.
 *         0 on success.
 *         Non-zero in case of errors or syntax issues.
 */
int	ft_handle_input_redirect(t_node *node, t_minishell *ms)
{
	int	fd;

	if (ft_check_redirect_syntax(node, ms))
		return (1);
	if (ft_validate_input_file(node, ms))
		return (1);
	fd = open(node->right->token->value, O_RDONLY);
	if (fd == -1)
		return (ft_handle_file_error(ms));
	if (dup2(fd, STDIN_FILENO) == -1)
		return (ft_handle_dup_error(fd, ms));
	close(fd);
	ft_set_exit_code(ms, 0);
	return (ft_execute_ast(node->left, ms));
}

/**
 * @brief  Validates the input file before opening it.
 * 
 * @param  node  Pointer to the input redirection node.
 * @param  ms    Pointer to the minishell structure.
 * @return int   1 if the file is invalid, 0 otherwise.
 */
static int	ft_validate_input_file(t_node *node, t_minishell *ms)
{
	if (ft_is_valid_file(node->right->token->value, O_RDONLY))
	{
		ft_remove_created_files(node->prev);
		ft_create_files(node->left);
		ft_set_exit_code(ms, 1);
		return (1);
	}
	return (0);
}

/**
 * @brief  Checks if the token value on the right side is invalid for
 * redirection or other syntax.
 * 
 * @param  value  Token value to check.
 * @return int    Boolean indicating validity.
 **         1 if invalid.
 **         0 if valid.
 */
int	ft_invalid_right_token_value(char *value)
{
	if (!value || ft_strcmp(value, ">") == 0 || ft_strcmp(value, ">>") == 0
		|| ft_strcmp(value, "<") == 0 || ft_strcmp(value, "<<") == 0
		|| ft_strcmp(value, "|") == 0 || ft_strcmp(value, "&&") == 0
		|| ft_strcmp(value, "||") == 0)
		return (1);
	return (0);
}

/**
 * @brief  Swaps the values of redirect nodes to handle nested or reversed cases.
 * 
 * @param  node  Pointer to the current node in the AST.
 * @param  type  Redirect type (input/output).
 */
void	ft_swap_redirects_values(t_node *node, t_type type)
{
	t_node	*current;
	char	*temp_value;

	if (!node || node->token->type != type)
		return ;
	current = node;
	while (current && current->left && current->left->token->type == type)
		current = current->left;
	if (current != node)
	{
		temp_value = current->right->token->value;
		current->right->token->value = node->right->token->value;
		node->right->token->value = temp_value;
	}
}
