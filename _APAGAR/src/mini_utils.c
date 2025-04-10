/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 02:09:28 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/02/27 03:27:50 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int ft_perror(char *error, int return_value);
int ft_putstr_and_return(char *msg, int return_value);
void	ft_set_exit_code(t_minishell *ms, int exit_code);
int		ft_exit_code(t_minishell *ms);
char *ft_strjoin_all(int argcount, ...);
char	*ft_strjoin_free(char *s1, char *s2, int free_s1, int free_s2);
static char	*ft_free_inputs(char *s1, char *s2, int free_s1, int free_s2);
void	ft_print_ast(t_node *node, int depth);


/**
 * @brief  Print an error message using perror and return a specified value.
 * 
 * @param  error         The error message to display.
 * @param  return_value  The value to return after displaying the error.
 * @return int           The specified return value.
 */
int ft_perror(char *error, int return_value)
{
	perror(error);
	return (return_value);
}

/**
 * @brief  Print a message to STDERR and return a specified value.
 * 
 * @param  msg           The message to display.
 * @param  return_value  The value to return after displaying the message.
 * @return int           The specified return value.
 */
int ft_putstr_and_return(char *msg, int return_value)
{
	ft_putstr_fd(msg, STDERR_FILENO);
	return (return_value);
}

/**
 * @brief Getter and Setter for Exit Codes
 * 
 * @param ms 
 * @param exit_code 
 */
void	ft_set_exit_code(t_minishell *ms, int exit_code)
{
	ms->exit_code = exit_code;
}

/**
 * @brief  Get the current exit code from the minishell structure.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return int The current exit code.
 */
int	ft_exit_code(t_minishell *ms)
{
	return (ms->exit_code);
}

/**
 * @brief  Concatenate multiple strings into a single string.
 * 
 * @param  argcount  The number of strings to concatenate.
 * @param  ...       Variadic arguments representing the strings to join.
 * @return char*     Pointer to the newly allocated concatenated string.
 **                  NULL if memory allocation fails.
 */
char *ft_strjoin_all(int argcount, ...)
{
	va_list	ap;
	char *tmp;
	char *to_free;
	char *curr_str;
	int curr_arg;

	curr_arg = 0;
	tmp = malloc(1);
	tmp[0] = 0;
	va_start(ap, argcount);
	while (curr_arg++ < argcount)
	{
		curr_str = va_arg(ap, char*);
		to_free = tmp;
		tmp = ft_strjoin(tmp, curr_str);
		free(to_free);
	}
	va_end(ap);
	return (tmp);
}

/* Temporaria para testar */
/**
 * @brief  Recursively prints the Abstract Syntax Tree (AST) structure for debugging.
 * 
 * @param  node   Pointer to the current node in the AST.
 * @param  depth  Current depth level for indentation.
 */
void	ft_print_ast(t_node *node, int depth)
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

/**
 * @brief  Joins two strings and optionally frees them.
 * 
 * This function concatenates `s1` and `s2` into a new dynamically allocated 
 * string. It also provides an option to free `s1` and/or `s2` after joining.
 * 
 * @param  s1       First string to join.
 * @param  s2       Second string to join.
 * @param  free_s1  If true, frees `s1` after joining.
 * @param  free_s2  If true, frees `s2` after joining.
 * @return char*    Newly allocated concatenated string or NULL on failure.
 */
char	*ft_strjoin_free(char *s1, char *s2, int free_s1, int free_s2)
{
	char	*joined;
	size_t	len1;
	size_t	len2;

	len1 = 0;
	len2 = 0;
	if (s1)
		len1 = ft_strlen(s1);
	if (s2)
		len2 = ft_strlen(s2);
	joined = malloc(len1 + len2 + 1);
	if (!joined)
		return (ft_free_inputs(s1, s2, free_s1, free_s2));
	if (s1)
		ft_strlcpy(joined, s1, len1 + 1);
	if (s2)
		ft_strlcat(joined, s2, len1 + len2 + 1);
	ft_free_inputs(s1, s2, free_s1, free_s2);
	return (joined);
}

/**
 * @brief  Frees input strings based on flags.
 * 
 * @param  s1       First string.
 * @param  s2       Second string.
 * @param  free_s1  If true, frees `s1`.
 * @param  free_s2  If true, frees `s2`.
 * @return NULL     Always returns NULL for easier error handling.
 */
static char	*ft_free_inputs(char *s1, char *s2, int free_s1, int free_s2)
{
	if (free_s1 && s1)
		free(s1);
	if (free_s2 && s2)
		free(s2);
	return (NULL);
}
