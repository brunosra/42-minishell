/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 11:32:18 by bschwell          #+#    #+#             */
/*   Updated: 2025/02/18 18:22:59 by bschwell         ###   ########.fr       */
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
