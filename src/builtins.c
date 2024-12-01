/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:54:19 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/12/01 22:31:15 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		ft_builtin_echo(char **args);
void	ft_builtin_pwd(); // deve retornar int
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

/**
 * @brief 		Builtin Echo
 * 
 * @param args	what should be written in the command line
 * @return int	exit_code
 */

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
	exit(0);
}

int ft_builtin_exit(char **args, t_minishell *ms)
{
	if (args[2][0])
	{
		ft_putstr_fd("exit\nbash: exit: too many arguments\n", STDERR_FILENO);
		exit(1);
	}
	if (args[1][0])
	{
		ms->exit_code = ft_atoi(args[1]);
		exit(ms->exit_code);
	} 
/* 	ft_free_tokens(ms->tokens);
	ft_free_ast(ms->ast_root);
	free(ms->input); */
	ms->exit_code = 0;
	exit(ms->exit_code); // Precisa limpar antes de sair!
}

/**
 * @brief	pwd - print name of current/working directory
 * 
 * @return int	exit_code
 * *			0	success
 * *			1	error
 * TODO:		Check if malloc() is needed on cwd var.
 */

int	ft_builtin_pwd() // Tem de retornar um int
{	
	char cwd[1024]; // VER!! malloc!?
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("CWD IS: %s\n", cwd);
	else
		perror("pwd");
	return (EX_OK);
	// }
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
