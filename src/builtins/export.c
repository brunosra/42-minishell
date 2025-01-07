/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 11:28:18 by bschwell          #+#    #+#             */
/*   Updated: 2025/01/06 16:34:31 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
extern volatile sig_atomic_t g_interrupt;

void	ft_dup_envp(char **src, char **dest, int count) {
    int i;

	i = 0;
    while (i < count) {
        dest[i] = malloc(ft_strlen(src[i]) + 1);
        if (!dest[i]) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
        ft_strcpy(dest[i], src[i]);
        i++;
    }

    dest[i] = NULL;
}

void	free_dup_envp(char **arr) {
	int i;

	i = 0;
	if (!arr)
		return;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void ft_swap_env(char **a, char **b)
{
	char *temp = *a;
	*a = *b;
	*b = temp;
}

void ft_sort_envp(char **arr, int count)
{
	int i;
	int j;

	i = 0;
	while (i < count - 1) {
		j = 0;
		while (j < count - i - 1) {
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
				swap(&arr[j], &arr[j + 1]);
			j++;
		}
		i++;
	}
}





/**
 * @brief	Check if arguments are valid for export
 * 
 * @param	arg		string with varname
 * @return	int 
 * *		0: ok
 * *		1: error
 */
static int ft_check_valid_varname(char *arg)
{
	if (!arg[0] || !ft_isalpha(arg[0]) || arg[0] == '_')
		return (1);
	printf("arg[0]: %c\n", arg[0]);
	return (0);
}

/**
 * @brief 	Check if argument is an option (starts with -)
 * 
 * @param 	arg arg to be checked
 * @return 	int
 * *		0: ok
 * *		1: error
 */
static int ft_check_option(char *arg)
{
	if (arg[0] == '-')
	{
		printf("export: options not implemented\n");
		return (1);
	}
	return (0);
}

/**
 * @brief	Check if arguments are valid for export
 * 
 * @param	arg		string with varname
 * @return	int 
 * *		0: ok!
 * *		1: error
 */
static int ft_check_valid_varname(char *arg)
{
	if (!arg[0] || !ft_isalpha(arg[0]) || arg[0] == '_')
		return (1);
	return (0);
}

void	ft_builtin_export(t_minishell *ms)
{
	char	**dupenv;
	int		count;

	while (src[count] != NULL) {
        count++;
    }
	if (args[1] == NULL)
	{
		// only 1 arg

	}
	printf("ms: %s", ms->env.env_paths);
	/* printf("########## envp: #########\n");
	ft_print_str_arr(ms->env.envp); */
	/* printf("####### env_paths: #######\n");
	printf("%s\n", ms->env.env_paths); */
	/* printf("######### paths: #########\n");
	ft_print_str_arr(ms->env.paths);
	printf("####### full_path: #######\n");
	printf("%s\n", ms->env.full_path);*/
}

/* 
#########################################
BASH HELP FOR EXPORT - FOR REFERENCE ONLY
#########################################

export: export [-fn] [name[=value] ...] or export -p
    Set export attribute for shell variables.
    
    Marks each NAME for automatic export to the environment of subsequently
    executed commands.  If VALUE is supplied, assign VALUE before exporting.
    
    Options:
      -f	refer to shell functions
      -n	remove the export property from each NAME
      -p	display a list of all exported variables and functions
    
    An argument of `--' disables further option processing.
    
    Exit Status:
    Returns success unless an invalid option is given or NAME is invalid.
 */