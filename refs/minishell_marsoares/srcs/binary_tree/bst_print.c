/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 08:54:20 by marsoare          #+#    #+#             */
/*   Updated: 2024/10/16 12:33:00 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	*print_bst(void *root, int space)
{
	t_node	*node;
	int		spacing;

	if (space == 5)
		printf("\n\n\nTREE VIEW:\n");
	spacing = 5;
	space += spacing;
	if (!root)
		return (NULL);
	node = (t_node *)root;
	if (node->type == N_PIPE)
		print_pipe_children((t_pipe *)root, space);
	else if (node->type == N_EXEC)
		print_bst_exec((t_exec *)root, space);
	else if (node->type == N_ANDIF)
		ltree_print((t_andif *)root, space);
	else if (node->type == N_OR)
		ltree_print((t_or *)root, space);
	return (NULL);
}

void	print_pipe_children(t_pipe *pipe, int space)
{
	if (pipe->right)
		print_bst(pipe->right, space);
	print_bst_pipe(pipe, space);
	if (pipe->left)
		print_bst(pipe->left, space);
}

void	print_bst_pipe(t_pipe *node, int space)
{
	int	i;

	i = 0;
	printf("\n");
	while (i < space)
	{
		printf(" ");
		i++;
	}
	if (node->type.type == N_PIPE)
		printf("[PIPE]\n");
}
