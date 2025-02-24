/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 11:32:18 by bschwell          #+#    #+#             */
/*   Updated: 2025/02/23 09:07:09 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// TODO: Decide if we want to delete or not this file afterwards

#include "../include/minishell.h"
extern volatile int	g_interrupt;

/**
 * @brief 		Prints an array of Strings
 * 
 * @param args 	the array to be printed
 */
void	ft_print_str_arr(char **arr)
{
	int i;

	i = -1;
	if (arr == NULL)
	{
		printf("[print_str_arr]: NULL / Empty array");
		return;
	}
	else
		while (arr[++i])
			printf("[%d]: %s\n", i, arr[i]);
}

/**
 * @brief	Swaps the values of two environment variable pointers.
 * 
 * @param	a	Pointer to the first string to be swapped.
 * @param	b	Pointer to the second string to be swapped.
 * 
 * @details
 * - Swaps the values of the two pointers using a temporary variable.
 */
void	ft_swap(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

/**
 * @brief	Frees the memory allocated for a duplicated
 * 			environment variable array.
 * 
 * @param	arr	Array of strings to be freed.
 * 
 * @details
 * - Frees each string in the array.
 * - Frees the array pointer itself.
 * - Does nothing if the array is NULL.
 */
void	ft_free_str_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
