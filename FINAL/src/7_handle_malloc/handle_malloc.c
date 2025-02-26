/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_malloc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 22:18:13 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/02/26 06:38:42 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_free_tokens(t_token *tokens);
void	ft_free_ast(t_node *root);
void	ft_free_split(char **str);

/**
 * @brief  Frees the memory allocated for the token array.
 * 
 * @param  tokens  Pointer to the array of tokens to free.
 */
void	ft_free_tokens(t_token *tokens)
{
	int	i;

	i = -1;
	if (!tokens)
		return ;
	while (tokens[++i].value)
	{
		free(tokens[i].value);
		if (tokens[i].old_value)
			free(tokens[i].old_value);
	}
	free(tokens);
	return ;
}

/**
 * @brief  Frees the memory allocated for the abstract syntax tree (AST).
 * 
 * @param  root  Pointer to the root of the AST to free.
 */
void	ft_free_ast(t_node *root)
{
	if (!root)
		return ;
	if (root->left)
		ft_free_ast(root->left);
	if (root->right)
		ft_free_ast(root->right);
	ft_free_split(root->cmd_ready);
	free(root);
	return ;
}

/**
 * @brief  Frees the memory allocated for a split string array.
 * 
 * @param  str  Pointer to the array of strings to free.
 */
void ft_free_split(char **str)
{
	int i;

	i = 0;
	if (!str || !*str)
		return ;
	while (str[i])
		i++;
	while (--i >= 0)
	{
		if (str[i][0] != '\0')
			free(str[i]);
	}
	free(str);
	return ;
}
