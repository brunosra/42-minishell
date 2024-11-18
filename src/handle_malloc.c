/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_malloc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 22:18:13 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/11/18 20:16:37 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_tokens(t_token *tokens);
void	ft_free_ast(t_node *root);
/* void	ft_free_env(t_env *env); */

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
	ft_free_split(root->cmd_ready);
	free(root);
	return ;
}

/* void	ft_free_env(t_env *env)
{
	if (env->full_path)
		free(env->full_path);
} */

void ft_free_split(char **str)
{
	int i;

	i = 0;
	if (!str || !*str)
		return ;
	while (str[i])
		i++;
	while (--i >= 0)
		free(str[i]);
	free(str);
	return ;
}