/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:54:19 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/11/20 00:27:17 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* void ft_builtin_echo(char **args);
void ft_builtin_pwd(void);
void ft_builtin_exit(char **args); */

/* JUST FOR TEST */
static void print_str_arr(char **args)
{
	int i;

	i = -1;
	while (args[++i])
		printf("[%d]: %s\n", i, args[i]);
}

void ft_builtin_echo(char **args)
{
	int i;
	int j;
	int newline;

	i = 1;
	j = 2;
	newline = 1;
	print_str_arr(args);
	while (args[i] && !ft_strncmp(args[i], "-n", 2))
	{
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] == '\0')
		{
			i++;
			newline = 0;
		}
		else 
			break ;	
	}
	while (args[i] && !ft_strncmp(args[i], "-n", 2))
	{
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] == '\0')
			i++;
		else
			break ;
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

void ft_builtin_env(char **args)
{
	char **env;

	env = args;
	while (*env != 0)
	{
		printf("%s\n", *env);
		env++;
	}
}

// void ft_builtin_cd(char *path)
// {
// 	int ret;

// 	if (path[1])
// 		ret = chdir(path);
// 	if (ret)
// }