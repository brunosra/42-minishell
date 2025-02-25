/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_ASTree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:58:48 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/02/25 16:14:30 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_node	*ft_parse_ast(t_token *tokens);
static void	ft_process_token(t_token *tokens, int *i, t_node **current,
				t_node **root);
static t_node	*ft_handle_op_node(t_token *tokens, int *i,
				t_node **current, t_node **root);
static void	ft_adjust_input_redirect(t_token *tokens, int *i,
				t_ast_helper *ast);
static t_node	*ft_handle_cmd_node(t_token *tokens, int *i, t_node **current,
				t_node **root);
t_node	*ft_create_cmd_node(t_token *token);
t_node	*ft_create_operator_node(t_token *token, t_node *left, t_node *right);
t_node	*ft_group_command_tokens(t_token *tokens, int *index);
static void	ft_process_cmd_value(t_node *cmd_node, t_cmd_helper *h);
static void	ft_fill_cmd_node(t_token *tokens, int *index, t_node *cmd_node,
								t_cmd_helper *h);
static void	ft_handle_individual_token(t_token *tokens, int *index,
										t_node *cmd_node, t_cmd_helper *h);
static void	ft_handle_redirects(t_token *tokens, int *index, t_cmd_helper *h);
static void	ft_collect_arguments(t_token *tokens, int *index, t_cmd_helper *h);
t_cmd_helper	ft_init_cmd_helper(t_node *cmd_node);
int		ft_verify_cmd_node_value(t_node *cmd_node);
char	*ft_remove_quotes(char *value);
static void	ft_concat_new_value(char **final, char *sub);
static char	*ft_extract_unquoted(char *value, int *index);
static char	*ft_extract_quoted(char *value, int *index, char quote_type);
char	**ft_remove_null_values(char **cmd_ready, int arg_count);
static void	ft_copy_valid_args(char **cmd_ready, char **new_cmd_ready,
					int valid_count);
static int	ft_count_valid_args(char **cmd_ready);
/* int ft_value_has_space(char *value);*/

/* Temporaria para testar */
void	print_ast(t_node *node, int depth);

// /**
//  * @brief  Parses tokens into an Abstract Syntax Tree (AST).
//  * 
//  * @param  tokens  Array of tokens to parse.
//  * @return t_node* Pointer to the root node of the AST.
//  */
// t_node *ft_parse_ast(t_token *tokens)
// {
//     int     i;
//     t_node  *root;
//     t_node  *current;
//     t_node  *cmd_node;
//     t_node  *op_node;

//     i = 0;
//     root = NULL;
//     current = NULL;
//     cmd_node = NULL;
//     op_node = NULL;
	
// 	while (/* tokens[i].value &&  */tokens[i].type != TKN_NULL) 
// 	{
//         // printf("token_value = %s\n", tokens[i].value);
// 		// printf("token_type = %d\n", tokens[i].type);
// 		if(!tokens[i].value && tokens[i].type == TKN_CMD)
// 		{	
// 			tokens[i].value = ft_strdup("");
// /* 			ft_putstr_fd(": command not found\n", STDERR_FILENO); // ou Command '' not found
// 			return (NULL); // Código de erro para "command not found" */
// 		}
// 		if (tokens[i].type == TKN_BLTIN || tokens[i].type == TKN_CMD || tokens[i].type == TKN_FILE  || tokens[i].type == TKN_ARG /*&& current->token->type != TKN_FILE && current->token->type != TKN_VAR && current->token->type != TKN_ARG) */)
// 		{
// 			if (tokens[i].type == TKN_FILE)
// 			{
// 				if (current && current->token->type == TKN_HDOC)
// 					tokens[i].old_value = ft_strdup(tokens[i].value);
// 				tokens[i].value = ft_remove_quotes(tokens[i].value);
// 			}
// 			if (tokens[i].type == TKN_ARG && current && current->token->type == TKN_FILE && current->prev->token->type == TKN_HDOC)
// 				tokens[i].type = TKN_CMD;
// 			if (tokens[i].type == TKN_BLTIN || tokens[i].type == TKN_CMD)
// 			{
// 				cmd_node = ft_group_command_tokens(tokens, &i);
// /* 				if (cmd_node == NULL)
// 				{												//ALTERAR// INPUT - "$CASA" | "$ CAMA" | "$USER" | '$HOME' $USER $ "$ HOME" << HOME
// 					cmd_node = ft_create_cmd_node(&tokens[i]); // verificar se tem heredocs e se tiver cpnecta o ramos esquerdo a este no, se nao tiver executa o comando!
// 					return (cmd_node);
// 				}  */
// 			}
// 			else
// 			{
// 				cmd_node = ft_create_cmd_node(&tokens[i]);
// 				i++;
// 			}
// 			// ft_putnbr_fd(tokens[i].type, 1);
//             if (!root)
//                 root = cmd_node;
// 			else if (current && (current->token->type == TKN_PIPE || current->token->type == TKN_OUT_RD || current->token->type == TKN_IN_RD || current->token->type == TKN_HDOC || current->token->type == TKN_EXCPT))
//                 current->right = cmd_node;
// 			else if (tokens[i].type != TKN_ARG && cmd_node->token->type != TKN_ARG)/* if (current && current->prev->token->type == TKN_IN_RD && tokens[i].type == TKN_CMD && current->token->type == TKN_FILE && !current->prev->left) */
// 			{
// 				current->prev->left = cmd_node;
// 			}
// 			cmd_node->prev = current;
//             current = cmd_node;
//         }
//         else if (tokens[i].type == TKN_PIPE || tokens[i].type == TKN_OUT_RD || tokens[i].type == TKN_IN_RD || tokens[i].type == TKN_HDOC || tokens[i].type == TKN_EXCPT)
// 		{
//             /* if (!ft_strcmp(tokens[i].value, "|"))
// 			{ */
//                 op_node = ft_create_operator_node(&tokens[i], root, NULL);
// 				op_node->prev = current;
// 				if (!root)
// 					root = op_node;
// 				else
// 				{
// 					root->prev = op_node;
// 				}
// 				if (tokens[i].type == TKN_IN_RD && current && current->prev && current->prev->right == current && current->prev->token->type == TKN_PIPE)
// 				{
// 					root = current->prev;
// 					root->right = op_node;
// 					op_node->left = current;
// 					op_node->prev = root;
// 					root->prev = NULL;
// 					current = op_node;
// 					i++;
// 				}
// /* 				else if (tokens[i].type == TKN_PIPE && current->prev->right == current && current->prev->token->type == TKN_IN_RD)
// 				{
// 					root = op_node;
// 					root->right = current->prev;
// 					op_node->left = current;
// 					op_node->prev = root;
// 					current = op_node;
// 					i++;
// 				} */
// 				else
// 				{
// 					root = op_node;
// 					current = op_node;
// 					i++;
// 				}
//             /* } */
// /* 			else if (!ft_strcmp(tokens[i].value, ">") || !ft_strcmp(tokens[i].value, "<") || !ft_strcmp(tokens[i].value, ">>") || !ft_strcmp(tokens[i].value, "<<"))
// 			{
// 				op_node = ft_create_operator_node(&tokens[i], root, NULL);
// 				root = op_node;
// 				current = op_node;
//             }
//             i++; */
//         }
// 		else
// 			i++;
//     }
//  	if(root)
// 		root->prev = NULL;
//     return (root);
// }

/**
 * @brief Handles command nodes during AST parsing.
 * 
 * @param tokens      Array of tokens to parse.
 * @param i           Pointer to the current index in tokens.
 * @param current     Pointer to the current node in the AST.
 * @param root        Pointer to the root node of the AST.
 * @return t_node*    Created command node.
 */
static t_node	*ft_handle_cmd_node(t_token *tokens, int *i, t_node **current,
									t_node **root)
{
	t_node	*cmd_node;

	if (tokens[*i].type == TKN_BLTIN || tokens[*i].type == TKN_CMD)
		cmd_node = ft_group_command_tokens(tokens, i);
	else
	{
		cmd_node = ft_create_cmd_node(&tokens[*i]);
		(*i)++;
	}
	if (!*root)
		*root = cmd_node;
	else if (*current && ((*current)->token->type == TKN_PIPE
		|| (*current)->token->type == TKN_OUT_RD
		|| (*current)->token->type == TKN_IN_RD
		|| (*current)->token->type == TKN_HDOC
		|| (*current)->token->type == TKN_EXCPT))
		(*current)->right = cmd_node;
	else if (tokens[*i].type != TKN_ARG
			&& cmd_node->token->type != TKN_ARG)
		(*current)->prev->left = cmd_node;
	cmd_node->prev = *current;
	*current = cmd_node;
	return (cmd_node);
}

/**
 * @brief Adjusts the AST structure when handling input redirection.
 * 
 * @param tokens      Array of tokens.
 * @param i           Pointer to the current index in tokens.
 * @param ast         Helper struct with pointers to AST nodes.
 */
static void	ft_adjust_input_redirect(t_token *tokens, int *i,
				t_ast_helper *ast)
{
	if (tokens[*i].type == TKN_IN_RD && *(ast->current)
		&& (*(ast->current))->prev
		&& (*(ast->current))->prev->right == *(ast->current)
		&& (*(ast->current))->prev->token->type == TKN_PIPE)
	{
		*(ast->root) = (*(ast->current))->prev;
		(*(ast->root))->right = ast->op_node;
		ast->op_node->left = *(ast->current);
		ast->op_node->prev = *(ast->root);
		(*(ast->root))->prev = NULL;
		*(ast->current) = ast->op_node;
		(*i)++;
	}
	else
	{
		*(ast->root) = ast->op_node;
		*(ast->current) = ast->op_node;
		(*i)++;
	}
}

/**
 * @brief Handles operator nodes during AST parsing.
 * 
 * @param tokens      Array of tokens to parse.
 * @param i           Pointer to the current index in tokens.
 * @param current     Pointer to the current node in the AST.
 * @param root        Pointer to the root node of the AST.
 * @return t_node*    Created operator node.
 */
static t_node	*ft_handle_op_node(t_token *tokens, int *i,
									t_node **current, t_node **root)
{
	t_ast_helper	ast;

	ast.current = current;
	ast.root = root;
	ast.op_node = ft_create_operator_node(&tokens[*i], *root, NULL);
	ast.op_node->prev = *current;
	if (!*root)
		*root = ast.op_node;
	else
		(*root)->prev = ast.op_node;
	ft_adjust_input_redirect(tokens, i, &ast);
	return (ast.op_node);
}

/**
 * @brief Handles different types of tokens while parsing AST.
 * 
 * @param tokens      Array of tokens.
 * @param i           Pointer to the current index in tokens.
 * @param current     Pointer to the current node in the AST.
 * @param root        Pointer to the root node of the AST.
 */
static void	ft_process_token(t_token *tokens, int *i, t_node **current,
				t_node **root)
{
	if (tokens[*i].type == TKN_BLTIN || tokens[*i].type == TKN_CMD
		|| tokens[*i].type == TKN_FILE
		|| tokens[*i].type == TKN_ARG)
	{
		if (tokens[*i].type == TKN_FILE /* && *current
			&& (*current)->token->type == TKN_HDOC */)
		{
			tokens[*i].old_value = ft_strdup(tokens[*i].value);
			tokens[*i].value = ft_remove_quotes(tokens[*i].value);
		}
		ft_handle_cmd_node(tokens, i, current, root);
	}
	else if (tokens[*i].type == TKN_PIPE
		|| tokens[*i].type == TKN_OUT_RD
		|| tokens[*i].type == TKN_IN_RD
		|| tokens[*i].type == TKN_HDOC
		|| tokens[*i].type == TKN_EXCPT)
		ft_handle_op_node(tokens, i, current, root);
	else
		(*i)++;
}

/**
 * @brief Parses tokens into an Abstract Syntax Tree (AST).
 * 
 * @param tokens      Array of tokens to parse.
 * @return t_node*    Pointer to the root node of the AST.
 */
t_node	*ft_parse_ast(t_token *tokens)
{
	int		i;
	t_node	*root;
	t_node	*current;

	i = 0;
	root = NULL;
	current = NULL;
	while (tokens[i].type != TKN_NULL)
	{
		if (!tokens[i].value && tokens[i].type == TKN_CMD)
			tokens[i].value = ft_strdup("");
		ft_process_token(tokens, &i, &current, &root);
	}
	if (root)
		root->prev = NULL;
	return (root);
}


// /**
//  * @brief  Groups command-related tokens into a single command node.
//  * 
//  * @param  tokens  Array of tokens.
//  * @param  index   Pointer to the current token index.
//  * @return t_node* Pointer to the created command node.
//  */
// t_node	*ft_group_command_tokens(t_token *tokens, int *index)
// {
// 	t_node	*cmd_node;
// 	int		arg_count;
// 	int		i;
// 	char 	**cmd_nd_value;
// 	int		j;
// 	int		n_args_cmd_nd_values;
// 	int		len_value;
// 	int		c_except;
// 	int		stop;
// 	int		empty;

// 	cmd_node = ft_create_cmd_node(&tokens[*index]);
// 	c_except = 0;
// 	empty = 0;
// 	cmd_nd_value = NULL;
// 	arg_count = 1;
// 	i = 0;
// 	j = 0;
// 	len_value = ft_strlen(cmd_node->token->value);
// 	n_args_cmd_nd_values = ft_verify_cmd_node_value(cmd_node);
// 	(*index)++;
// 	while (tokens[*index].value && (tokens[*index].type == TKN_ARG || tokens[*index].type == TKN_VAR))
// 	{
// 		if (tokens[*index].value[0] == '\0')
// 		{
// 			(*index)++;
// 			empty++;
// 		}	
// 		else
// 		{
// 			arg_count++;
// 			(*index)++;
// 		}
// 		// printf("%i = %s\n", *index, tokens[*index].value);
// 	}
// 	stop = *index;
// 	while (tokens[*index].value && (tokens[*index].type != TKN_PIPE && tokens[*index].type != TKN_EXCPT && tokens[*index].type != TKN_CMD))
// 	{
// 		if (tokens[*index].type == TKN_IN_RD || tokens[*index].type == TKN_OUT_RD || tokens[*index].type == TKN_HDOC || tokens[*index].type == TKN_FILE)
// 		{
// 			c_except++;
// 			(*index)++;
// 			if (tokens[*index].type == TKN_FILE)
// 			{
// 				c_except++;
// 				(*index)++;
// 			}
// 		}
// 		while (tokens[*index].value && (tokens[*index].type == TKN_ARG || tokens[*index].type == TKN_VAR))
// 		{
// 			if (tokens[*index].value[0] == '\0')
// 			{
// 				(*index)++;
// 				empty++;
// 			}	
// 			else
// 			{
// 				arg_count++;
// 				(*index)++;
// 			}
// 		// printf("%i = %s\n", *index, tokens[*index].value);
// 		}
// 	}
// 	cmd_node->cmd_ready = malloc(sizeof(char *) * (arg_count + (n_args_cmd_nd_values - 1) + 1));
// 	if (!cmd_node->cmd_ready)
// 		return (NULL);
// 	if (n_args_cmd_nd_values > 1)
// 	{
// 		cmd_nd_value = ft_split(cmd_node->token->value, ' ');
// 		while (cmd_nd_value[j])
// 		{
// 			cmd_node->cmd_ready[j] = ft_strdup(cmd_nd_value[j]);
// 			j++;
// 		}
// 		ft_free_split(cmd_nd_value);
// 		arg_count--;
// 	}
// 	else if (ft_cmp_str_str(cmd_node->token->value, "\"", len_value) || ft_cmp_str_str(cmd_node->token->value, "\'", len_value))
// 		cmd_node->token->value = ft_remove_quotes(cmd_node->token->value);
// 	*index -= (arg_count + c_except + empty); // Ajusta o índice para voltar ao início do comando
// 	i = j;
// 	if (n_args_cmd_nd_values != 1)
// 		arg_count += n_args_cmd_nd_values;
// 	while (i < arg_count)
// 	{
// 		if (tokens[*index].value && (tokens[*index].type == TKN_ARG || tokens[*index].type == TKN_VAR || tokens[*index].type == TKN_CMD || tokens[*index].type == TKN_BLTIN))
// 		{
// 			if (tokens[*index].value[0] == '\0')
// 			{
// 				(*index)++;
// 				continue ;		
// 			}	// cmd_node->cmd_ready[i] = ft_strdup(""); // talvez seja para mudar!
// 			if (i > j)
// 				tokens[*index].value = ft_remove_quotes(tokens[*index].value);
// 			if (tokens[*index].value/* [0] != '\0' */)
// 				cmd_node->cmd_ready[i] = ft_strdup(tokens[*index].value);
// 			i++;
// 			(*index)++;
// 		}
// 		else if ((tokens[*index].type == TKN_IN_RD || tokens[*index].type == TKN_OUT_RD || tokens[*index].type == TKN_HDOC || tokens[*index].type == TKN_FILE))
// 			(*index)++;
// 		else
// 			break ;
// 		// 	return (NULL);
// 	}
// 	if (c_except)
// 		*index = stop;
// 	cmd_node->cmd_ready[i] = NULL;
// 	cmd_node->cmd_ready = ft_remove_null_values(cmd_node->cmd_ready, arg_count);
// 	return (cmd_node);
// }

/**
 * @brief  Initializes a helper structure to group command tokens.
 * 
 * This function initializes a structure that is used to keep track of
 * various states during the process of handling command tokens, including
 * the argument count, token value length, and the number of arguments.
 * 
 * @param cmd_node  A pointer to the node representing the command.
 * 
 * @return t_cmd_helper  The initialized helper structure.
 */
t_cmd_helper	ft_init_cmd_helper(t_node *cmd_node)
{
	t_cmd_helper h;

	h.cmd_nd_value = NULL;
	h.arg_count = 1;
	h.i = 0;
	h.j = 0;
	h.len_value = ft_strlen(cmd_node->token->value);
	h.n_args_cmd_nd_values = ft_verify_cmd_node_value(cmd_node);
	h.c_except = 0;
	h.empty = 0;
	return (h);
}

/**
 * @brief  Counts arguments and handles empty values.
 * 
 * This function processes the tokens from the provided index and counts
 * the number of arguments, incrementing the argument count for valid
 * tokens while handling empty values by incrementing a separate counter.
 * 
 * @param tokens  An array of tokens.
 * @param index   A pointer to the current index in the tokens array.
 * @param h       A pointer to the helper structure.
 */
static void	ft_collect_arguments(t_token *tokens, int *index, t_cmd_helper *h)
{
	while (tokens[*index].value && 
		(tokens[*index].type == TKN_ARG || 
		tokens[*index].type == TKN_VAR))
	{
		if (tokens[*index].value[0] == '\0')
		{
			(*index)++;
			h->empty++;
		}
		else
		{
			h->arg_count++;
			(*index)++;
		}
	}
}

/**
 * @brief  Handles redirects and counts special cases.
 * 
 * This function processes tokens that represent redirects (input,
 * output, heredoc) or filenames and updates the helper structure
 * accordingly, including special handling for redirects.
 * 
 * @param tokens  An array of tokens.
 * @param index   A pointer to the current index in the tokens array.
 * @param h       A pointer to the helper structure.
 */
static void	ft_handle_redirects(t_token *tokens, int *index, t_cmd_helper *h)
{
	while (tokens[*index].value && 
		(tokens[*index].type != TKN_PIPE && 
		tokens[*index].type != TKN_EXCPT &&
		tokens[*index].type != TKN_CMD))
	{
		if (tokens[*index].type == TKN_IN_RD || 
			tokens[*index].type == TKN_OUT_RD || 
			tokens[*index].type == TKN_HDOC || 
			tokens[*index].type == TKN_FILE)
		{
			h->c_except++;
			(*index)++;
			if (tokens[*index].type == TKN_FILE)
			{
				h->c_except++;
				(*index)++;
			}
		}
		ft_collect_arguments(tokens, index, h);
	}
}

/**
 * @brief  Handles an individual token and updates the command node's `cmd_ready`.
 * 
 * This helper function processes an individual token, checking if the
 * token value is empty and skipping it if necessary. It also removes
 * quotes from the token's value if required and then copies the value
 * to the command node's `cmd_ready` array.
 * 
 * @param tokens   An array of tokens.
 * @param index    A pointer to the current index in the tokens array.
 * @param cmd_node A pointer to the command node to be filled.
 * @param h        A pointer to the helper structure.
 */
static void	ft_handle_individual_token(t_token *tokens, int *index,
										t_node *cmd_node, t_cmd_helper *h)
{
	if (tokens[*index].value[0] == '\0')
	{
		(*index)++;
		return;
	}
	if (h->i > h->j)
		tokens[*index].value = ft_remove_quotes(tokens[*index].value);
	cmd_node->cmd_ready[h->i] = ft_strdup(tokens[*index].value);
	h->i++;
	(*index)++;
}

/**
 * @brief  Fills the command node with token values.
 * 
 * This function fills the command node's `cmd_ready` array with token
 * values from the tokens array. It processes argument and redirection
 * tokens, removing quotes from arguments if necessary.
 * 
 * @param tokens   An array of tokens.
 * @param index    A pointer to the current index in the tokens array.
 * @param cmd_node A pointer to the command node to be filled.
 * @param h        A pointer to the helper structure.
 */
static void	ft_fill_cmd_node(t_token *tokens, int *index, t_node *cmd_node,
								t_cmd_helper *h)
{
	h->i = h->j;
	if (h->n_args_cmd_nd_values != 1)
		h->arg_count += h->n_args_cmd_nd_values;
	while (h->i < h->arg_count)
	{
		if (tokens[*index].value && (tokens[*index].type == TKN_ARG
			|| tokens[*index].type == TKN_VAR 
			|| tokens[*index].type == TKN_CMD
			|| tokens[*index].type == TKN_BLTIN))
		{
			ft_handle_individual_token(tokens, index, cmd_node, h);
		}
		else if ((tokens[*index].type == TKN_IN_RD
				|| tokens[*index].type == TKN_OUT_RD
				|| tokens[*index].type == TKN_HDOC
				|| tokens[*index].type == TKN_FILE))
			(*index)++;
		else
			break;
	}
}

/**
 * @brief  Processes the command node value, handling spaces and quotes.
 * 
 * This function processes the value of the command node, handling spaces
 * by splitting the value into multiple parts and quotes by removing them
 * from the value.
 * 
 * @param cmd_node A pointer to the command node.
 * @param h        A pointer to the helper structure.
 */
static void	ft_process_cmd_value(t_node *cmd_node, t_cmd_helper *h)
{
	if (h->n_args_cmd_nd_values > 1)
	{
		h->cmd_nd_value = ft_split(cmd_node->token->value, ' ');
		while (h->cmd_nd_value[h->j])
		{
			cmd_node->cmd_ready[h->j] = ft_strdup(h->cmd_nd_value[h->j]);
			h->j++;
		}
		ft_free_split(h->cmd_nd_value);
		h->arg_count--;
	}
	else if (ft_cmp_str_str(cmd_node->token->value, "\"", h->len_value)
			|| ft_cmp_str_str(cmd_node->token->value, "'", h->len_value))
		cmd_node->token->value = ft_remove_quotes(cmd_node->token->value);
}

/**
 * @brief  Groups command-related tokens into a single command node.
 * 
 * This function groups tokens related to a single command, processing
 * arguments, redirects, and command values, and returns a command node
 * that represents the complete command structure with associated values.
 * 
 * @param tokens  An array of tokens.
 * @param index   A pointer to the current index in the tokens array.
 * 
 * @return t_node*  A pointer to the command node.
 */
t_node	*ft_group_command_tokens(t_token *tokens, int *index)
{
	t_node		*cmd_node;
	t_cmd_helper h;

	cmd_node = ft_create_cmd_node(&tokens[*index]);
	h = ft_init_cmd_helper(cmd_node);
	(*index)++;
	ft_collect_arguments(tokens, index, &h);
	h.stop = *index;
	ft_handle_redirects(tokens, index, &h);
	cmd_node->cmd_ready = malloc(sizeof(char *) * (h.arg_count + 
		(h.n_args_cmd_nd_values - 1) + 1));
	if (!cmd_node->cmd_ready)
		return (NULL);
	ft_process_cmd_value(cmd_node, &h);
	*index -= (h.arg_count + h.c_except + h.empty);
	ft_fill_cmd_node(tokens, index, cmd_node, &h);
	if (h.c_except)
		*index = h.stop;
	cmd_node->cmd_ready[h.i] = NULL;
	cmd_node->cmd_ready = ft_remove_null_values(cmd_node->cmd_ready,
												h.arg_count);
	return (cmd_node);
}

// /**
//  * @brief  Removes null or empty values from a command's arguments array.
//  * 
//  * @param  cmd_ready  Array of command arguments.
//  * @param  arg_count  Total number of arguments.
//  * @return char**     Updated array without null or empty values.
//  */
// char **ft_remove_null_values(char **cmd_ready, int arg_count)
// {
// 	int i;
// 	int j;
// 	int c;
// 	char **new_cmd_ready;

// 	i = 0;
// 	j = 0;
// 	c = 0;
// 	new_cmd_ready = NULL;
// 	while (cmd_ready[i] != NULL)
// 	{
// 	//	printf("cmd_ready[i][0] = %c\n", cmd_ready[i][0]);
// 		if (cmd_ready[i][0] == '\0')
// 			i++;
// 		else
// 		{
// 			i++;
// 			c++;
// 		}
// 	}
// 	if (c == arg_count)
// 		return (cmd_ready);
// 	new_cmd_ready = malloc(c + 1);
// 	i = 0;
// 	while (c > 0)
// 	{
// 		if (cmd_ready[i][0] == '\0')
// 			i++;
// 		else
// 		{
// 			new_cmd_ready[j] = ft_strdup(cmd_ready[i]);
// 			j++;
// 			i++;
// 			c--;
// 		}	
// 	}
// 	new_cmd_ready[j] = NULL;
// 	if (*cmd_ready)
// 		ft_free_split(cmd_ready);
// 	return (new_cmd_ready);
// }

/**
 * @brief  Counts the number of valid (non-null, non-empty) values in cmd_ready.
 * 
 * @param  cmd_ready  Array of command arguments.
 * @return int        Number of valid arguments.
 */
static int	ft_count_valid_args(char **cmd_ready)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (cmd_ready[i])
	{
		if (cmd_ready[i][0] != '\0')
			count++;
		i++;
	}
	return (count);
}

/**
 * @brief  Copies valid arguments into a new allocated array.
 * 
 * @param  cmd_ready      Array of command arguments.
 * @param  new_cmd_ready  New allocated array for valid arguments.
 * @param  valid_count    Number of valid arguments.
 */
static void	ft_copy_valid_args(char **cmd_ready, char **new_cmd_ready,
								int valid_count)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (valid_count > 0)
	{
		if (cmd_ready[i][0] != '\0')
		{
			new_cmd_ready[j] = ft_strdup(cmd_ready[i]);
			j++;
			valid_count--;
		}
		i++;
	}
	new_cmd_ready[j] = NULL;
}

/**
 * @brief  Removes null or empty values from a command's arguments array.
 * 
 * @param  cmd_ready  Array of command arguments.
 * @param  arg_count  Total number of arguments.
 * @return char**     Updated array without null or empty values.
 */
char	**ft_remove_null_values(char **cmd_ready, int arg_count)
{
	char	**new_cmd_ready;
	int		valid_count;

	valid_count = ft_count_valid_args(cmd_ready);
	if (valid_count == arg_count)
		return (cmd_ready);
	new_cmd_ready = malloc(sizeof(char *) * (valid_count + 1));
	if (!new_cmd_ready)
		return (cmd_ready);
	ft_copy_valid_args(cmd_ready, new_cmd_ready, valid_count);
	if (*cmd_ready)
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

// /**
//  * @brief  Removes quotes from a given string and returns the updated value.
//  * 
//  * @param  value  String to process and remove quotes from.
//  * @return char*  Processed string with quotes removed.
//  */
// char	*ft_remove_quotes(char *value)
// {
// 	int		i;
// 	int		start;
// 	char	quote_type;
// 	char	*new_value;
// 	char	*temp;
// 	char	*sub;

// 	i = 0;
// 	new_value = NULL;
// 	while (value[i])
// 	{
// 		if (value[i] == '\'' || value[i] == '\"')
// 		{
// 			quote_type = value[i++];
// 			start = i;
// 			while (value[i] && value[i] != quote_type)
// 				i++;
// 			sub = ft_substr(value, start, i - start);
// 			if (new_value == NULL)
// 				new_value = ft_strdup(sub);
// 			else
// 			{
// 				temp = new_value;
// 				new_value = ft_strjoin(new_value, sub);
// 				free(temp);
// 			}
// 			free(sub);
// 			if (value[i])
// 				i++;
// 		}
// 		else
// 		{
// 			start = i;
// 			while (value[i] && value[i] != '\'' && value[i] != '\"')  // VER O QUE ALTERA!
// 				i++;
// 			sub = ft_substr(value, start, i - start);
// 			if (new_value == NULL)
// 				new_value = ft_strdup(sub);
// 			else
// 			{
// 				temp = new_value;
// 				new_value = ft_strjoin(new_value, sub);
// 				free(temp);
// 			}
// 			free(sub);
// 		}
// 	}
// 	if (value)
// 		free(value);
// 	return (new_value);
// }

/**
 * @brief  Extracts a substring between matching quotes.
 * 
 * @param  value       Input string containing quotes.
 * @param  index       Pointer to current position in string.
 * @param  quote_type  Type of quote (' or ").
 * @return char*       Extracted substring without quotes.
 */
static char	*ft_extract_quoted(char *value, int *index, char quote_type)
{
	int		start;
	char	*sub;

	start = ++(*index);
	while (value[*index] && value[*index] != quote_type)
		(*index)++;
	sub = ft_substr(value, start, *index - start);
	if (value[*index])
		(*index)++;
	return (sub);
}

/**
 * @brief  Extracts a substring without quotes.
 * 
 * @param  value  Input string.
 * @param  index  Pointer to current position in string.
 * @return char*  Extracted substring.
 */
static char	*ft_extract_unquoted(char *value, int *index)
{
	int		start;

	start = *index;
	while (value[*index] && value[*index] != '\'' && value[*index] != '\"')
		(*index)++;
	return (ft_substr(value, start, *index - start));
}

/**
 * @brief  Concatenates a new substring to the final string.
 * 
 * @param  final  Pointer to the final string.
 * @param  sub    Substring to append.
 */
static void	ft_concat_new_value(char **final, char *sub)
{
	char	*temp;

	if (!*final)
		*final = ft_strdup(sub);
	else
	{
		temp = *final;
		*final = ft_strjoin(*final, sub);
		free(temp);
	}
	free(sub);
}

/**
 * @brief  Removes quotes from a given string and returns the updated value.
 * 
 * @param  value  String to process and remove quotes from.
 * @return char*  Processed string with quotes removed.
 */
char	*ft_remove_quotes(char *value)
{
	int		i;
	char	*final;
	char	*sub;

	i = 0;
	final = NULL;
	while (value[i])
	{
		if (value[i] == '\'' || value[i] == '\"')
			sub = ft_extract_quoted(value, &i, value[i]);
		else
			sub = ft_extract_unquoted(value, &i);
		ft_concat_new_value(&final, sub);
	}
	free(value);
	return (final);
}


/**
 * @brief  Verifies the number of arguments within a command node value.
 * 
 * @param  cmd_node  Command node to evaluate.
 * @return int       Number of arguments found within the command node value.
 */
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

/**
 * @brief  Creates a new command node in the AST.
 * 
 * @param  token  Token associated with the command node.
 * @return t_node* Pointer to the created command node.
 */
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
	node->file = false;
	node->heredoc_stops = NULL;
	return (node);
}

/**
 * @brief  Creates a new operator node in the AST.
 * 
 * @param  token  Token associated with the operator node.
 * @param  left   Pointer to the left child node.
 * @param  right  Pointer to the right child node.
 * @return t_node* Pointer to the created operator node.
 */
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
	node->file = false;
	node->heredoc_stops = NULL;
    return (node);
}

/* Temporaria para testar */
/**
 * @brief  Recursively prints the Abstract Syntax Tree (AST) structure for debugging.
 * 
 * @param  node   Pointer to the current node in the AST.
 * @param  depth  Current depth level for indentation.
 */
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
    	printf("Node Type: %d, Value: %s\n", node->token->type,
				node->token->value);
    print_ast(node->left, depth + 1);
    print_ast(node->right, depth + 1);
}
