/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 21:13:47 by bschwell          #+#    #+#             */
/*   Updated: 2025/02/23 08:55:57 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern volatile int	g_interrupt;
/* void	ft_dup_envp(char **src, char **dest, int count);
void	free_dup_envp(char **arr);
void	ft_swap_env(char **a, char **b);
void	ft_sort_envp(char **arr, int count); */

/**
 * @brief	Duplicates a given array of environment variables.
 * 
 * @param	src		Source array of strings to be duplicated.
 * @param	dest	Destination array to store the duplicated strings.
 * @param	count	Number of strings in the source array to duplicate.
 * 
 * @details
 * - Allocates memory for each string in the destination array.
 * - Copies the content of each string from the source array
 * 	 to the destination array.
 * - Adds a NULL terminator at the end of the destination array.
 * - Exits the program if memory allocation fails.
 */
int	ft_dup_envp(char **envp, char ***dupenv, int count)
{
	int	i;

	i = 0;
	*dupenv = malloc((count + 1) * sizeof(char *));
	if (!(*dupenv))
		return (-1);
	while (i < count)
	{
		(*dupenv)[i] = ft_strdup(envp[i]);
		if (!(*dupenv)[i])
		{
			while (i > 0)
			{
				i--;
				free((*dupenv)[i]);
			}
			free(*dupenv);
			return (-1);
		}
		i++;
	}
	(*dupenv)[count] = NULL;
	return (0);
}

/**
 * @brief	Frees the memory allocated for a duplicated
 * 			environment variable array.
 * 
 * @param	arr	Array of strings to be freed.
 * 
 * @details
 * - Frees each string in the array.
 * - Frees the array pointer itself.
 * - Does nothing if the array is NULL.
 */
void	free_dup_envp(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

/**
 * @brief	Swaps the values of two environment variable pointers.
 * 
 * @param	a	Pointer to the first string to be swapped.
 * @param	b	Pointer to the second string to be swapped.
 * 
 * @details
 * - Swaps the values of the two pointers using a temporary variable.
 */
static void	ft_swap(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

/**
 * @brief	Sorts an array of environment variable strings in alphabetical order.
 * 
 * @param	arr		Array of strings to be sorted.
 * @param	count	Number of strings in the array.
 * 
 * @details
 * - Implements a bubble sort algorithm to sort the strings lexicographically.
 * - Swaps adjacent strings if they are out of order.
 * - Uses `ft_strcmp` to compare the strings.
 */
int	ft_sort_envp(char **dupenv, int count)
{
	int	swapped;
	int	i;

	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (i < count - 1)
		{
			if (ft_strcmp(dupenv[i], dupenv[i + 1]) > 0)
			{
				ft_swap(&dupenv[i], &dupenv[i + 1]);
				swapped = 1;
			}
			i++;
		}
	}
	return (0);
}

/**
 * @brief	Check if arguments are valid for export
 * 
 * @param	arg		string with varname
 * @return	int 
 * *		0: ok
 * *		1: error
 */
/* static int ft_check_valid_varname(char *arg)
{
	if (!arg[0] || !ft_isalpha(arg[0]) || arg[0] == '_')
		return (1);
	printf("arg[0]: %c\n", arg[0]);
	return (0);
} */

/**
 * @brief 	Check if argument is an option (starts with -)
 * 
 * @param 	arg arg to be checked
 * @return 	int
 * *		0: ok
 * *		1: error
 */
/* static int	ft_check_option(char *arg)
{
	if (arg[0] == '-')
	{
		printf("export: options not implemented\n");
		return (1);
	}
	return (0);
} */

// TODO: implement
int	ft_builtin_export_check(char **args, t_minishell *ms)
{
	int		count;

	count = 0;
	while (args[count] != NULL)
	{
		count++;
	}
	printf("export check: %s\n", ms->env.envp[0]);
	return (0);
}

static void	ft_output_export(char **arr, int count)
{
	int i;

	i = -1;
	if (arr == NULL)
	{
		printf("[print_str_arr]: NULL / Empty array");
		return;
	}
	else
		while (++i < count - 1)
			printf("declare -x %s\n", arr[i]);
}

static void	ft_print_static_vars(t_minishell *ms)
{
	char	**dupenv;
	int		count;

	count = -1;
	while (ms->env.envp[++count] != NULL)
		;
	dupenv = malloc(sizeof(char *) * count);
	if (dupenv == NULL)
	{
		perror("malloc error");
		return ;
	}
	ft_dup_envp(ms->env.envp, &dupenv, count);
	ft_sort_envp(dupenv, count);
	ft_output_export(dupenv, count);
	/* printf("ms: %s", ms->env.env_paths); */
	
}

void	ft_builtin_export(char **args, t_minishell *ms)
{

	
	if (args[1] == NULL)
		ft_print_static_vars(ms);
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