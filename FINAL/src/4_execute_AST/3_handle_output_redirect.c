/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_handle_output_redirect.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 02:29:26 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/02/27 00:52:47 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int			ft_handle_output_redirect(t_node *node, t_minishell *ms);
int			ft_check_redirect_syntax(t_node *node, t_minishell *ms);
static int	ft_open_output_file(t_node *node);
int			ft_handle_file_error(t_minishell *ms);
int			ft_handle_dup_error(int fd, t_minishell *ms);

/**
 * @brief  Handles output redirection to a file.
 * 
 * @param  node  Pointer to the output redirection node in the AST.
 * @param  ms    Pointer to the minishell structure.
 * @return int   Execution status.
 *         0 on success.
 *         Non-zero in case of errors or syntax issues.
 */
int	ft_handle_output_redirect(t_node *node, t_minishell *ms)
{
	int	fd;

	if (ft_check_redirect_syntax(node, ms))
		return (1);
	fd = ft_open_output_file(node);
	if (fd == -1)
		return (ft_handle_file_error(ms));
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (ft_handle_dup_error(fd, ms));
	close(fd);
	if (node->left)
		return (ft_execute_ast(node->left, ms));
	ft_set_exit_code(ms, 0);
	return (0);
}

/**
 * @brief  Checks for syntax errors in output redirection.
 * 
 * @param  node  Pointer to the output redirection node.
 * @param  ms    Pointer to the minishell structure.
 * @return int   1 if there is a syntax error, 0 otherwise.
 */
int	ft_check_redirect_syntax(t_node *node, t_minishell *ms)
{
	if (!node->right)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token"
			"`newline'\n", STDERR_FILENO);
		ft_set_exit_code(ms, 2);
		return (1);
	}
	if (ft_invalid_right_token_value(node->right->token->value))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `",
			STDERR_FILENO);
		if (node->right->token->value)
			ft_putstr_fd(node->right->token->value, STDERR_FILENO);
		else
			ft_putstr_fd("newline", STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
		ft_set_exit_code(ms, 2);
		return (1);
	}
	return (0);
}

/**
 * @brief  Opens the file for output redirection.
 * 
 * @param  node  Pointer to the output redirection node.
 * @return int   File descriptor, or -1 on failure.
 */
static int	ft_open_output_file(t_node *node)
{
	int	fd;

	if (ft_strcmp(node->token->value, ">>") == 0)
		fd = open(node->right->token->value,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(node->right->token->value,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd != -1)
		node->file = true;
	return (fd);
}

/**
 * @brief  Handles errors when opening the file.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return int Always returns 1 to indicate an error.
 */
int	ft_handle_file_error(t_minishell *ms)
{
	perror("open");
	ft_set_exit_code(ms, 1);
	return (1);
}

/**
 * @brief  Handles errors when duplicating the file descriptor.
 * 
 * @param  fd  File descriptor.
 * @param  ms  Pointer to the minishell structure.
 * @return int Always returns 1 to indicate an error.
 */
int	ft_handle_dup_error(int fd, t_minishell *ms)
{
	perror("dup2");
	close(fd);
	ft_set_exit_code(ms, 1);
	return (1);
}
