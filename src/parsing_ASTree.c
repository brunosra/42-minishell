/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_ASTree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:58:48 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/11/10 21:38:53 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_node	*ft_parse_ast(t_token *tokens);
t_node	*ft_create_cmd_node(t_token *token);
t_node	*ft_create_operator_node(t_token *token, t_node *left, t_node *right);
t_node	*ft_group_command_tokens(t_token *tokens, int *index);
/* Temporaria para testar */
void	print_ast(t_node *node, int depth);

t_node *ft_parse_ast(t_token *tokens)
{
    int     i;
    t_node  *root;
    t_node  *current;
    t_node  *cmd_node;
    t_node  *op_node;

    i = 0;
    root = NULL;
    current = NULL;
    cmd_node = NULL;
    op_node = NULL;
	while (tokens[i].value)
	{
        if (tokens[i].type == TOKEN_COMMAND || tokens[i].type == TOKEN_FILENAME)
		{
            // Agrupa o comando e argumentos em um único nó de comando
            if (tokens[i].type == TOKEN_COMMAND)
				cmd_node = ft_group_command_tokens(tokens, &i);
			else
			{
				cmd_node = ft_create_cmd_node(&tokens[i]);
				i++;
			}
            if (!root)
			{
                root = cmd_node;
            }
			else if (current && (current->token->type == TOKEN_OPERATOR || current->token->type == TOKEN_OUTPUT_REDIRECT))
			{
                current->right = cmd_node;
            }
            current = cmd_node;
        }
        else if (tokens[i].type == TOKEN_OPERATOR || tokens[i].type == TOKEN_OUTPUT_REDIRECT || tokens[i].type == TOKEN_INPUT_REDIRECT)
		{
            // Processa operadores como |, >, <, >>, <<
            if (!ft_strcmp(tokens[i].value, "|"))
			{
                op_node = ft_create_operator_node(&tokens[i], root, NULL);
                root = op_node;
                current = op_node;
            }
			else if (!ft_strcmp(tokens[i].value, ">") || !ft_strcmp(tokens[i].value, "<") ||
                       !ft_strcmp(tokens[i].value, ">>") || !ft_strcmp(tokens[i].value, "<<"))
			{
				op_node = ft_create_operator_node(&tokens[i], root, NULL);
				root = op_node;
				current = op_node;
            }
            i++;
        }
    }
    return (root);
}

// Função para agrupar tokens em um único nó de comando
/* t_node *ft_group_command_tokens(t_token *tokens, int *index)
{
    t_node *cmd_node;
	char	*temp;
    
	temp = NULL;
	cmd_node = ft_create_cmd_node(&tokens[*index]);
	(*index)++;
    while (tokens[*index].value && tokens[*index].type == TOKEN_ARGUMENT)
	{
        temp = ft_strjoin(cmd_node->token->value, " ");
	//	write(1, cmd_node->token->value, ft_strlen(cmd_node->token->value));
	//	free(cmd_node->token->value);
		cmd_node->token->value = ft_strjoin(temp, tokens[*index].value);
        free(temp); 
        (*index)++;
    }
	cmd_node->cmd_ready = ft_split(cmd_node->token->value, ' ');
    return (cmd_node);
}
 */

t_node	*ft_group_command_tokens(t_token *tokens, int *index)
{
	t_node	*cmd_node;
	int		arg_count;
	int		i;

	// Cria o nó de comando
	cmd_node = ft_create_cmd_node(&tokens[*index]);

	// Conta o número de argumentos e o comando inicial
	arg_count = 1;
	(*index)++;
	while (tokens[*index].value && tokens[*index].type == TOKEN_ARGUMENT)
	{
		arg_count++;
		(*index)++;
	}

	// Aloca `cmd_ready` para o comando e argumentos + espaço para o NULL final
	cmd_node->cmd_ready = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd_node->cmd_ready)
		return (NULL);

	// Reinicia o índice para armazenar comando e argumentos
	*index -= arg_count; // Ajusta o índice para voltar ao início do comando
	i = 0;
	while (i < arg_count)
	{
		cmd_node->cmd_ready[i] = ft_strdup(tokens[*index].value);
	//	write(1, cmd_node->cmd_ready[i], ft_strlen(cmd_node->cmd_ready[i]));
		i++;
		(*index)++;
	}
	cmd_node->cmd_ready[i] = NULL; // Finaliza com NULL para o formato execve

	return (cmd_node);
}


t_node	*ft_create_cmd_node(t_token *token)
{
	t_node *node;

	node = malloc(sizeof(t_node));
	if (!node) 
		return (NULL);
	node->cmd_ready = NULL;	
	node->token = token;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_node	*ft_create_operator_node(t_token *token, t_node *left, t_node *right)
{
    t_node *node;

	node = malloc(sizeof(t_node));
    if (!node)
		return (NULL);
	node->cmd_ready = NULL;	
    node->token = token;
    node->left = left;
    node->right = right;
    return (node);
}

/* Temporaria para testar */
void print_ast(t_node *node, int depth)
{
    int i;
	int j;

	i = -1;
	j = 0;
	if (!node)
		return;
    while (++i < depth) 
		printf("  ");
	if (node->cmd_ready)
	{
		printf("Node Type: %d, Value:", node->token->type);
		while (node->cmd_ready[j])
		{
			printf(" %i_%s", j, node->cmd_ready[j]);
			j++;
		}
		printf("\n");
	}
	else
    	printf("Node Type: %d, Value: %s\n", node->token->type, node->token->value);
    print_ast(node->left, depth + 1);
    print_ast(node->right, depth + 1);
}
