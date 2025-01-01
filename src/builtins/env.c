/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 17:28:29 by bschwell          #+#    #+#             */
/*   Updated: 2024/12/31 17:28:35 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
extern volatile sig_atomic_t g_interrupt;

// TODO: conversar sobre o sair do programa aqui
void 	ft_builtin_env(char **args, t_minishell *ms)
{
	char **env;
	
	env = ms->env.envp;
	if (args[1])
		exit(printf("[env error]: doesn't support arguments\n")); // Talvez fosse melhor ignorar os restabtes argumentos! Ou enviar uma mensagem de erro mas nao sair do programa!
	else
		while (*env != 0)
		{
			printf("%s\n", *env);
			env++;
		}
	set_exit_code(ms, EX_OK);
	exit(exit_code(ms));
}
