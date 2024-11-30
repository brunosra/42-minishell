/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:54:19 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/11/29 18:51:45 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		ft_builtin_echo(char **args);
void	ft_builtin_pwd(t_minishell *ms); // deve retornar int
int		ft_builtin_exit(char **args, t_minishell *ms);
void 	ft_builtin_env(t_minishell *ms); // deve retornar int
/* int		ft_builtin_cd(t_minishell *ms);
 */

/* JUST FOR TEST */
static void print_str_arr(char **args)
{
	int i;

	i = -1;
	while (args[++i])
		printf("[%d]: %s\n", i, args[i]);
}

int ft_builtin_echo(char **args)
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
	return (0);
}

int ft_builtin_exit(char **args, t_minishell *ms)
{
	if (args[1])
		ms->exit_code = ft_atoi(args[1]); 
/* 	ft_free_tokens(ms->tokens);
	ft_free_ast(ms->ast_root);
	free(ms->input); */
	return (ms->exit_code); // Precisa limpar antes de sair!
}

void ft_builtin_pwd(t_minishell *ms) // Tem de retornar um int
{	
	char cwd[1024]; // VER!! malloc!?

	if (ms->ast_root->cmd_ready[1])
		exit(printf("[pwd error]: doesn't support arguments\n")); // O pwd do bash ignora os restantes argumentos!
	else
	{
		if (getcwd(cwd, sizeof(cwd)) != NULL)
			printf("CWD IS: %s\n", cwd);
		else
			perror("pwd");
		return ;
	}
}

void ft_builtin_env(t_minishell *ms) // Tem de retornar um int
{
	char **env;
	
	env = ms->env.envp;
	if (ms->ast_root->cmd_ready[1])
	{
		exit(printf("[env error]: doesn't support arguments\n")); // Talvez fosse melhor ignorar os restabtes argumentos! Ou enviar uma mensagem de erro mas nao sair do programa!
	}
	else
	{
		while (*env != 0)
		{
			printf("%s\n", *env);
			env++;
		}
	}
}

/* int ft_builtin_cd(t_minishell *ms)
{
	if (cd ..)
		exit(printf("[cd error]: too many arguments\n"));
	if (!ms->ast_root->cmd_ready[1])
		return (chdir("~"));
	else
		return (chdir(ms->ast_root->cmd_ready[1]));
} */
