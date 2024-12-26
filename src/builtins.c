/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:54:19 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/12/26 12:09:36 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
extern volatile sig_atomic_t g_interrupt;

void	ft_builtin_echo(char **args, t_minishell *ms);
void	ft_builtin_pwd(t_minishell *ms);
void	ft_builtin_exit(char **args, t_minishell *ms);
int		ft_value_is_numeric(char *str);
long long ft_atoll(char *str, int i, long long res);

void 	ft_builtin_env(char **args, t_minishell *ms); // deve retornar int
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

void	ft_builtin_echo(char **args, t_minishell *ms)
{
	int i;
	int newline;

	i = 1;
	newline = 1;
	while (ft_strncmp(args[i], "-n", 2) == 0)
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	set_exit_code(ms, 0);
}

/**
 * TODO: Tiago, Review esse echo que voce fez e compara se ha algo faltando no meu por favor.
 * 
 * 
 */


/* void	ft_builtin_echo(char **args, t_minishell *ms)
{
	int i;
	int j;
	int newline;

	i = 1;
	j = 2;
	newline = 1;
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
	set_exit_code(ms, 0);
} */

void	ft_builtin_exit(char **args, t_minishell *ms)
{
	int mod;
	int i;

	mod = 0;
	i = 0;
	while (args[i])	
		i++;
	if (i == 1)
		set_exit_code(ms, 0);
	else if (i > 2 && ft_atoll(args[1], 0, 0))
	{
		ft_putstr_fd("exit\nminishell: exit: too many arguments\n", STDERR_FILENO);
		exit(1);
	}
	else if (ft_value_is_numeric(args[1]))
	{
		mod = ft_atoll(args[1], 0, 0) % 256;
		set_exit_code(ms, mod);
		exit(exit_code(ms));
	}
	else if (!ft_value_is_numeric(args[1]))
	{
		ft_putstr_fd("exit\nminishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		exit(2);
	}
	else
		set_exit_code(ms, 0);
	exit(exit_code(ms));
}

long long ft_atoll(char *str, int i, long long res)
{
    int sig;

    sig = 1;
    while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
        i++;
    if (str[i] == '+' || str[i] == '-')
    {
        if (str[i] == '-')
            sig = -1;
        i++;
    }
    if (str[i] >= '0' && str[i] <= '9')
	{
		while (str[i] >= '0' && str[i] <= '9')
    	{
        	if (res > (LLONG_MAX / 10) || (res == LLONG_MAX / 10 && (str[i] - '0') > (LLONG_MAX % 10)))
        	{
				ft_putstr_fd("exit\nminishell: exit: ", STDERR_FILENO);
				ft_putstr_fd(str, STDERR_FILENO);
				ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
				exit(2);
			}
        	res = res * 10 + (str[i] - '0');
        	i++;
    	}
		if (str[i] != '\0')
		{
			ft_putstr_fd("exit\nminishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(str, STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			exit(2);
		}
	}
	else
	{
		ft_putstr_fd("exit\nminishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		exit(2);
	}
	return (res * sig);
}

int ft_value_is_numeric(char *str)
{
	int i;

	i = 0;	
	while (str[i])
	{
		if (ft_isdigit(str[i]))
			return (1);
		i++;
	}
	return (0);
}

// void ft_builtin_pwd(t_minishell *ms) // Tem de retornar um int
/**
 * @brief	pwd - print name of current/working directory
 * 
 * @return int	exit_code
 * *			0	success
 * *			1	error
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

void 	ft_builtin_env(char **args, t_minishell *ms)
{
	char **env;
	
	env = ms->env.envp;
	if (args[1])
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
	exit(0);
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
