/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 02:54:19 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/12/28 09:13:39 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
extern volatile sig_atomic_t g_interrupt;

void	ft_builtin_echo(char **args, t_minishell *ms);
void	ft_builtin_pwd(t_minishell *ms);
void	ft_builtin_exit(char **args, t_minishell *ms);
int		ft_value_is_numeric(char *str);
long long ft_atoll(char *str, int i, long long res);

void 	ft_builtin_env(char **args, t_minishell *ms);

/**
 * @brief 		Check if arg from echo is a valid variation of -n
 * 
 * @param opt	Option that needs to be checked
 * @return int	1: is a valid variation of -n
 * 				0: is not
 */

static int		ft_check_valid_echo_opt(char *opt)
{
	int	i;

	i = 1;
	if (ft_strncmp(opt, "-n", 2) == 0)
	{
		while (opt[i])
		{
			if (opt[i] != 'n')
				return (0);
			i++;
		}
	}
	else
		return (0);
	return (1);
}

/**
 * @brief 		Builtin echo
 * 
 * @param args 	What should be writter on the command line
 * @param ms 	minishell struct pointer
 */

void	ft_builtin_echo(char **args, t_minishell *ms)
{
	int i;
	int newline;

	i = 1;
	newline = 1;
	while (args[i])
	{
		if (ft_check_valid_echo_opt(args[i]) == 0)
			break;
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

// TODO: verificar se este echo acima esta ok, se estiver, apagar o de baixo

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

// TODO: Criar funcao de limpeza de memoria (valgrind) e chamar ela antes de todo exit
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

// TODO: conversar sobre o sair do programa aqui
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
	set_exit_code(ms, EX_OK);
	exit(exit_code(ms));
}
