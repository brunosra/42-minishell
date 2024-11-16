/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:54:19 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/11/14 20:28:33 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* void ft_builtin_echo(char **args);
void ft_builtin_pwd(void);
void ft_builtin_exit(char **args); */

void ft_builtin_echo(char **args)
{
	int i;
	int newline;

	i = 1;
	newline = 1;
	if (args[i] && !ft_strcmp(args[i], "-n"))
	{
		newline = 0;
		i = 2;
	}
	while (args[i])
	{
		if (args[i])
		{
			printf("%s", args[i]);
			if (args[i + 1])
				printf(" ");
			i++;
		}
	}
	if (newline)
		printf("\n");
	return ;
}

void ft_builtin_pwd(void)
{
	char cwd[1024]; // VER!! malloc!?

	if (getcwd(cwd, sizeof(cwd)))
		printf("%s\n", cwd);
	else
		perror("pwd");
	return ;
}

void ft_builtin_exit(char **args)
{
	int exit_code;

	exit_code = 0;
	if (args[1])
		exit_code = ft_atoi(args[1]);
	exit(exit_code);
}

void ft_builtin_cd(char *path)
{
	int ret;

	if (path[1])
		ret = chdir(path);
	if (ret)
}