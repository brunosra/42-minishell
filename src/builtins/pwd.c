/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 09:13:42 by bschwell          #+#    #+#             */
/*   Updated: 2025/02/18 18:22:59 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
extern volatile int	g_interrupt;

int	ft_builtin_pwd();

/**
 * @brief	pwd - print name of current/working directory
 * 
 * @param 	ms minishell pointer
 * @return 	int 
 */
int	ft_builtin_pwd()
{	
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
	{
		perror("pwd: ");
		return (errno);
	}
}
