/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 17:03:36 by hluiz-ma          #+#    #+#             */
/*   Updated: 2024/11/03 20:26:54 by hluiz-ma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	reset_printed(t_list *lst)
{
	t_list	*curr;
	t_env	*env;

	curr = lst;
	while (curr)
	{
		env = curr->content;
		env->printed = false;
		curr = curr->next;
	}
}

t_list	*get_next_var(t_list *lst)
{
	t_list	*curr;
	t_list	*smallest;
	t_env	*curr_env;
	t_env	*small_env;

	smallest = NULL;
	curr = lst;
	while (curr)
	{
		curr_env = curr->content;
		if (curr_env->printed == false)
		{
			if (!smallest)
				smallest = curr;
			else
			{
				small_env = smallest->content;
				if (ft_strcmp(curr_env->value, small_env->value) < 0)
					smallest = curr;
			}
		}
		curr = curr->next;
	}
	return (smallest);
}

void	print_var(t_env *env)
{
	if (!env->content)
		printf("declare -x %s\n", env->value);
	else
	{
		printf("declare -x %s", env->value);
		printf("\"%s\"\n", env->content);
	}
}

void	print_export_lst(t_list *lst)
{
	t_list	*curr;
	t_env	*env;
	int		i;
	int		size;

	size = ft_lstsize(lst);
	i = -1;
	while (++i < size)
	{
		curr = get_next_var(lst);
		if (!curr)
			break ;
		env = curr->content;
		env->printed = true;
		print_var(env);
	}
	reset_printed(lst);
}

void	mark_isexport(t_shell *shell, const char *var_name)
{
	t_list	*current;
	t_env	*env_var;
	t_env	*new_env;

	current = shell->envp;
	while (current)
	{
		env_var = current->content;
		if (is_exact_var(env_var, var_name))
		{
			env_var->is_export = true;
			return ;
		}
		current = current->next;
	}
	new_env = malloc(sizeof(t_env));
	if (!new_env)
		exit_failure(shell, "mark_isenv");
	new_env->value = ft_strdup(var_name);
	new_env->content = NULL;
	new_env->is_export = true;
	new_env->printed = false;
	ft_lstadd_back(&shell->envp, ft_lstnew(new_env));
}
