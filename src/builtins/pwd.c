/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 09:13:42 by bschwell          #+#    #+#             */
/*   Updated: 2024/12/28 09:14:27 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
extern volatile sig_atomic_t g_interrupt;

// void ft_builtin_pwd(t_minishell *ms) // Tem de retornar um int
/**
 * @brief	pwd - print name of current/working directory
 * 
 * @return	void
 * TODO:		Check if malloc() is needed on cwd var.
 */

void	ft_builtin_pwd(t_minishell *ms)
{	
	char cwd[4095];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("pwd: ");
	set_exit_code(ms, EX_OK);
	exit(exit_code(ms));
}
