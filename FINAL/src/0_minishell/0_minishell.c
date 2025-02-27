/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_minishell.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 23:48:24 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/02/27 06:38:44 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int			main(int argc, char **argv, char **envp);
static void	ft_init_ms(t_minishell *ms);
static int	ft_save_stdin_stdout(t_minishell *ms);
static int	ft_readline(t_minishell *ms);
void		ft_create_prompt(t_minishell *ms);

/**
 * @brief  Main entry point of the program.
 * 
 * @param  argc  Argument count (unused).
 * @param  argv  Argument vector (unused).
 * @param  envp  Environment variables.
 * @return int
 **        Exit code of the shell
 */
int	main(int argc, char **argv, char **envp)
{
	t_minishell	ms;

	(void)argv;
	(void)argc;
	ft_init_ms(&ms);
	ms.env.envp = ft_duplicate_envp(envp);
	if (!ms.env.envp)
		return (ft_perror("malloc", 1));
	while (1)
	{
		ft_set_main_signals();
		ft_ms_struct(&ms, 0);
		if (ft_save_stdin_stdout(&ms))
			return (1);
		if (ft_readline(&ms))
			break ;
		if (ft_process_input_and_execute(&ms))
			continue ;
		ft_free_tokens(ms.tokens);
		ft_free_ast(ms.ast_root);
		free(ms.input);
	}
	free(ms.input);
	ft_free_split(ms.env.envp);
	return (0);
}

/**
 * @brief  Initialize the minishell structure with default values.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return void
 */
static void	ft_init_ms(t_minishell *ms)
{
	ms->input = NULL;
	ms->tokens = NULL;
	ms->temp = NULL;
	ms->status = -1;
	ms->save_stdin = -1;
	ms->save_stdout = -1;
	ms->n_args = -1;
	ms->pid = -1;
	ms->exit_code = 0;
	ms->c_multi_heredocs = 0;
	ms->c_stuck_cats = 0;
	ms->pipefd[0] = -1;
	ms->pipefd[1] = -1;
	ms->swap_input_redirects = false;
	ms->swap_output_redirects = false;
	ms->in_pipe = false;
	ms->env.envp = NULL;
	ms->env.env_paths = NULL;
	ms->env.paths = NULL;
	ms->env.full_path = NULL;
	ft_init_prompt(ms);
}

/**
 * @brief  Save the current stdin and stdout file descriptors.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return int
 **        0 on success
 **        1 if an error occurs during duplication
 */
static int	ft_save_stdin_stdout(t_minishell *ms)
{
	ms->save_stdin = dup(STDIN_FILENO);
	ms->save_stdout = dup(STDOUT_FILENO);
	if (ms->save_stdin == -1 || ms->save_stdout == -1)
		return (ft_perror("dup", 1));
	return (0);
}

/**
 * @brief  Read a line of input using readline and handle the prompt display.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return int
 **        0 on successful input
 **        1 if the input is NULL (EOF or Ctrl-D)
 */
static int	ft_readline(t_minishell *ms)
{
	ft_create_prompt(ms);
	ms->swap_output_redirects = false;
	ms->swap_input_redirects = false;
	ms->input = readline(ms->prompt);
	if (ms->input == NULL)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		free(ms->prompt);
		return (1);
	}
	if (ms->input)
		add_history(ms->input);
	return (0);
}

/**
 * @brief  Create a new prompt string based on the current path and exit code.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return void
 */
void	ft_create_prompt(t_minishell *ms)
{
	char	*old_prompt;
	char	*new_prompt;
	char	*e;
	char	p[PATH_MAX];

	e = ft_itoa(ft_exit_code(ms));
	getcwd(p, PATH_MAX);
	old_prompt = ms->prompt;
	new_prompt = ft_strjoin_all(6,
			RD"["RST, e, RD"] ["RST, p, RD"] minishell"RST, "$ ");
	free(old_prompt);
	free(e);
	ms->prompt = new_prompt;
}
