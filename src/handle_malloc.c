/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_malloc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 22:18:13 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/12/11 06:02:43 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_free_tokens(t_token *tokens);
void	ft_free_ast(t_node *root);
void	ft_free_split(char **str);
char	*ft_strjoin_free(char *s1, char *s2, int free_s1, int free_s2); // mover para utils ou libft
/* void	ft_free_env(t_env *env); */

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
	{
//		ft_putstr_fd(str[i], 1);
		if (str[i][0] != '\0')
			free(str[i]);
	}
	free(str);
	return ;
}

char	*ft_strjoin_free(char *s1, char *s2, int free_s1, int free_s2) // mover para utils ou libft
{
	char	*joined;
	size_t	len1;
	size_t	len2;

	len1 = 0;
	len2 = 0;
	if (!s1 && !s2)
		return (NULL);
	if (s1)
		len1 = ft_strlen(s1);
	if (s2)
		len2 = ft_strlen(s2);
	joined = malloc(len1 + len2 + 1);
	if (!joined)
	{
		if (free_s1 && s1)
			free(s1);
		if (free_s2 && s2)
			free(s2);
		return (NULL);
	}
	if (s1)
		ft_strlcpy(joined, s1, len1 + 1);
	if (s2)
		ft_strlcat(joined, s2, len1 + len2 + 1);
	if (free_s1 && s1)
		free(s1);
	if (free_s2 && s2)
		free(s2);
	return (joined);
}
