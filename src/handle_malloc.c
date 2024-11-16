/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_malloc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <bschwell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 22:18:13 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/11/16 12:27:23 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_tokens(t_token *tokens);
void	ft_free_ast(t_node *root);
void	ft_free_cmd_ready(char **cmd_ready);

void	ft_free_tokens(t_token *tokens)
{
	int	i;

	i = -1;
	if (!tokens)
		return ;
	while (tokens[++i].value)
		free(tokens[i].value);
	free(tokens);
	return ;
}

void	ft_free_ast(t_node *root)
{
	if (!root)
		return ;
	if (root->left)
		ft_free_ast(root->left);
	if (root->right)
		ft_free_ast(root->right);
	ft_free_cmd_ready(root->cmd_ready);
	free(root);
	return ;
}

void	ft_free_cmd_ready(char **cmd_ready)
{
	int	i;

	if (!cmd_ready)
		return ;
	i = -1;
	while (cmd_ready[++i])
		free(cmd_ready[i]);
	free(cmd_ready);
	return ;
}
