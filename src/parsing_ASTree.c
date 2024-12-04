/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_ASTree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:58:48 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/12/04 06:23:48 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_node	*ft_parse_ast(t_token *tokens);
t_node	*ft_create_cmd_node(t_token *token);
t_node	*ft_create_operator_node(t_token *token, t_node *left, t_node *right);
t_node	*ft_group_command_tokens(t_token *tokens, int *index);
int		ft_verify_cmd_node_value(t_node *cmd_node);
char	*ft_remove_quotes(char *value);
char 	**ft_remove_null_values(char **cmd_ready, int arg_count);
/* int ft_value_has_space(char *value);*/

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
	
	while (/* tokens[i].value  || */tokens[i].type != TOKEN_NULL) 
	{
        // printf("token_value = %s\n", tokens[i].value);
		// printf("token_type = %d\n", tokens[i].type);
		if(!tokens[i].value && tokens[i].type == TOKEN_COMMAND)
		{	
			tokens[i].value = ft_strdup("");
/* 			ft_putstr_fd(": command not found\n", STDERR_FILENO); // ou Command '' not found
			return (NULL); // Código de erro para "command not found" */
		}
		if (tokens[i].type == TOKEN_BUILTIN || tokens[i].type == TOKEN_COMMAND || tokens[i].type == TOKEN_FILENAME /* || (tokens[i].type == TOKEN_VARIABLE && current->token->type != TOKEN_FILENAME && current->token->type != TOKEN_VARIABLE && current->token->type != TOKEN_ARGUMENT) */)
		{
			if (tokens[i].type == TOKEN_FILENAME)
			{
				tokens[i].value = ft_remove_quotes(tokens[i].value);
			}
			if (tokens[i].type == TOKEN_BUILTIN || tokens[i].type == TOKEN_COMMAND)
				cmd_node = ft_group_command_tokens(tokens, &i);
			else
			{
				cmd_node = ft_create_cmd_node(&tokens[i]);
				i++;
			}
            if (!root)
                root = cmd_node;
			else if (current && (current->token->type == TOKEN_OPERATOR || current->token->type == TOKEN_OUTPUT_REDIRECT || current->token->type == TOKEN_INPUT_REDIRECT || current->token->type == TOKEN_HEREDOC))
                current->right = cmd_node;
			else /* if (current && current->prev->token->type == TOKEN_INPUT_REDIRECT && tokens[i].type == TOKEN_COMMAND && current->token->type == TOKEN_FILENAME && !current->prev->left) */
			{
				current->prev->left = cmd_node;
			}
			cmd_node->prev = current;
            current = cmd_node;
        }
        else if (tokens[i].type == TOKEN_OPERATOR || tokens[i].type == TOKEN_OUTPUT_REDIRECT || tokens[i].type == TOKEN_INPUT_REDIRECT || tokens[i].type == TOKEN_HEREDOC)
		{
            /* if (!ft_strcmp(tokens[i].value, "|"))
			{ */
                op_node = ft_create_operator_node(&tokens[i], root, NULL);
				op_node->prev = current;
				if (!root)
					root = op_node;
				else
				{
					root->prev = op_node;
				}
				root = op_node;
				current = op_node;
				i++;
            /* } */
/* 			else if (!ft_strcmp(tokens[i].value, ">") || !ft_strcmp(tokens[i].value, "<") || !ft_strcmp(tokens[i].value, ">>") || !ft_strcmp(tokens[i].value, "<<"))
			{
				op_node = ft_create_operator_node(&tokens[i], root, NULL);
				root = op_node;
				current = op_node;
            }
            i++; */
        }
		else
			i++;
    }
    return (root);
}

t_node	*ft_group_command_tokens(t_token *tokens, int *index)
{
	t_node	*cmd_node;
	int		arg_count;
	int		i;
	char 	**cmd_nd_value;
	int		j;
	int		n_args_cmd_nd_values;
	int		len_value;
	int		c_except;
	int		stop;

	cmd_node = ft_create_cmd_node(&tokens[*index]);
	c_except = 0;
	cmd_nd_value = NULL;
	arg_count = 1;
	i = 0;
	j = 0;
	len_value = ft_strlen(cmd_node->token->value);
	n_args_cmd_nd_values = ft_verify_cmd_node_value(cmd_node);
	(*index)++;
	while (tokens[*index].value && (tokens[*index].type == TOKEN_ARGUMENT || tokens[*index].type == TOKEN_VARIABLE))
	{
		arg_count++;
		(*index)++;
	}
	stop = *index;
	while (tokens[*index].value && tokens[*index].type != TOKEN_OPERATOR)
	{
		if (tokens[*index].type == TOKEN_INPUT_REDIRECT || tokens[*index].type == TOKEN_OUTPUT_REDIRECT || tokens[*index].type == TOKEN_HEREDOC)
		{
			c_except++;
			(*index)++;
			if (tokens[*index].type == TOKEN_FILENAME)
			{
				c_except++;
				(*index)++;
			}
/* 			else
				exit(1); // ver melhor */
		}
		while (tokens[*index].value && (tokens[*index].type == TOKEN_ARGUMENT || tokens[*index].type == TOKEN_VARIABLE))
		{
			arg_count++;
			(*index)++;
		}
	}
	cmd_node->cmd_ready = malloc(sizeof(char *) * (arg_count + (n_args_cmd_nd_values - 1) + 1));
	if (!cmd_node->cmd_ready)
		return (NULL);
	if (n_args_cmd_nd_values > 1)
	{
		cmd_nd_value = ft_split(cmd_node->token->value, ' ');
		while (cmd_nd_value[j])
		{
			cmd_node->cmd_ready[j] = ft_strdup(cmd_nd_value[j]);
			j++;
		}
		ft_free_split(cmd_nd_value);
		arg_count--;
	}
	else if (/* *cmd_node->token->value == '"' || *cmd_node->token->value == '\'' */ft_cmp_str_str(cmd_node->token->value, "\"", len_value) || ft_cmp_str_str(cmd_node->token->value, "\'", len_value))
		cmd_node->token->value = ft_remove_quotes(cmd_node->token->value);
	*index -= (arg_count + c_except); // Ajusta o índice para voltar ao início do comando
	i = j;
	if (n_args_cmd_nd_values != 1)
		arg_count += n_args_cmd_nd_values;
	while (i < arg_count)
	{
		if (tokens[*index].value && (tokens[*index].type == TOKEN_ARGUMENT || tokens[*index].type == TOKEN_VARIABLE || tokens[*index].type == TOKEN_COMMAND || tokens[*index].type == TOKEN_BUILTIN))
		{
			if (i > j)
			{
				// if (tokens[*index].value[0] == '"' || tokens[*index].value[0] == '\'')
					tokens[*index].value = ft_remove_quotes(tokens[*index].value);
				if (tokens[*index].value)
					cmd_node->cmd_ready[i] = ft_strdup(tokens[*index].value);
				else
					cmd_node->cmd_ready[i] = ft_strdup(""); // talvez seja para mudar!
			}
			else
			{
/* 				tokens[*index].value = ft_remove_quotes(tokens[*index].value);
 */				if (tokens[*index].value)
					cmd_node->cmd_ready[i] = ft_strdup(tokens[*index].value);
				else
					cmd_node->cmd_ready[i] = ft_strdup(""); // talvez seja para mudar!
			}
			i++;
			(*index)++;
		}
		else if ((tokens[*index].type == TOKEN_INPUT_REDIRECT || tokens[*index].type == TOKEN_OUTPUT_REDIRECT || tokens[*index].type == TOKEN_HEREDOC || tokens[*index].type == TOKEN_FILENAME))
			(*index)++;
/* 		else
			exit (0);// ver melhor */
	}
	if (c_except)
		*index = stop;
	cmd_node->cmd_ready[i] = NULL;
	cmd_node->cmd_ready = ft_remove_null_values(cmd_node->cmd_ready, arg_count);
	return (cmd_node);
}

char **ft_remove_null_values(char **cmd_ready, int arg_count)
{
	int i;
	int j;
	int c;
	char **new_cmd_ready;

	i = 0;
	j = 0;
	c = 0;
	new_cmd_ready = NULL;
	while (cmd_ready[i] != NULL)
	{
		// printf("cmd_ready[i][0] = %c\n", cmd_ready[i][0]);
		if (cmd_ready[i][0] == '\0')
			i++;
		else
		{
			i++;
			c++;
		}
	}
	if (c == arg_count)
		return (cmd_ready);
	new_cmd_ready = malloc(c + 1);
	i = 0;
	while (c > 0)
	{
		if (cmd_ready[i][0] == '\0')
			i++;
		else
		{
			new_cmd_ready[j] = ft_strdup(cmd_ready[i]);
			j++;
			i++;
			c--;
		}	
	}
	new_cmd_ready[j] = NULL;
	ft_free_split(cmd_ready);
	return (new_cmd_ready);
}


/* int ft_value_has_space(char *value)
{
	int i;

	i = 0;
	if (!*value)
		return (-1);
	while (value[i] && value[i] != ' ')
		i++;
	if (value[i] == '\0')
		return (0);
	return (1);
	
}
 */

char	*ft_remove_quotes(char *value)
{
	int		i;
	int		start;
	char	quote_type;
	char	*new_value;
	char	*temp;
	char	*sub;

	i = 0;
	new_value = NULL;
	while (value[i])
	{
		if (value[i] == '\'' || value[i] == '\"')
		{
			quote_type = value[i++];
			start = i;
			while (value[i] && value[i] != quote_type)
				i++;
			sub = ft_substr(value, start, i - start);
			if (new_value == NULL)
				new_value = ft_strdup(sub);
			else
			{
				temp = new_value;
				new_value = ft_strjoin(new_value, sub);
				free(temp);
			}
			free(sub);
			if (value[i])
				i++;
		}
		else
		{
			start = i;
			while (value[i] && value[i] != '\'' && value[i] != '\"')  // VER O QUE ALTERA!
				i++;
			sub = ft_substr(value, start, i - start);
			if (new_value == NULL)
				new_value = ft_strdup(sub);
			else
			{
				temp = new_value;
				new_value = ft_strjoin(new_value, sub);
				free(temp);
			}
			free(sub);
		}
	}
	free(value);
	return (new_value);
}

int ft_verify_cmd_node_value(t_node *cmd_node)
{
	int i;
	int count;
	char *value;

	i = 0;
	count = 0;
	value = cmd_node->token->value;
	if (value[i] == '"' || value[i] == '\'')
		return (1);
	while (value[i])
	{
		if (value[i] && value[i] != ' ')
			count++;
		while (value[i] && value[i] != ' ')
			i++;
		while (value[i] && value[i] == ' ')
			i++;	
	}
	return (count);
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
	node->prev = NULL;
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
	node->prev = NULL;
    return (node);
}

/* Temporaria para testar */
void	print_ast(t_node *node, int depth)
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
