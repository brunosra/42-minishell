/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 23:31:41 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/02/24 19:24:22 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
// TODO: nao podemos usar esta variavel global para ter os sinais.
// TODO: Somente podemos usar um int para receber. nao podemos usar sig_atomic_t
volatile int	g_interrupt;

int	main(int argc, char **argv, char **envp);
void ft_init_ms(t_minishell *ms);
int ft_save_stdin_stdout(t_minishell *ms);
int	ft_readline(t_minishell *ms);
void ft_close_stdin_stdout(t_minishell *ms);
int	ft_handle_and_tokenize_input(t_minishell *ms);
int	ft_process_input_and_execute(t_minishell *ms);
void ft_clean_stuck_cats(t_minishell *ms);
void ft_find_stuck_cats(t_minishell *ms, t_node *node);

/**
 * @brief  Initialize the minishell structure with default values.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return void
 */
void ft_init_ms(t_minishell *ms)
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
	ms->env.env_paths = ft_calloc(1, sizeof(char *));
	ms->env.paths = ft_calloc(1, sizeof(char **));
	ms->env.full_path = ft_calloc(1, sizeof(char *));
	getcwd(ms->currpath, PATH_MAX);
	ms->prompt = ft_strjoin_all(4, RD"["RST, ft_itoa(ft_exit_code(ms)), RD"] minishell"RST, "$ ");
	// ms->prompt = RD"minishell:"RST"$";
}

/**
 * @brief  Save the current stdin and stdout file descriptors.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return int
 **        0 on success
 **        1 if an error occurs during duplication
 */
int ft_save_stdin_stdout(t_minishell *ms)
{
	ms->save_stdin = dup(STDIN_FILENO);
	ms->save_stdout = dup(STDOUT_FILENO);
	if (ms->save_stdin == -1 || ms->save_stdout == -1)
		return (ft_perror("dup", 1));
	return (0);
}

/**
 * @brief  Close saved stdin and stdout file descriptors.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return void
 */
void ft_close_stdin_stdout(t_minishell *ms)
{
	close(ms->save_stdin);
	close(ms->save_stdout);
}

/**
 * @brief  Create a new prompt string based on the current path and exit code.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return void
 */
void ft_create_prompt(t_minishell *ms)
{
	char *old_prompt;
	char *new_prompt;
	char *e;
	char p[PATH_MAX];

	e = ft_itoa(ft_exit_code(ms));
	getcwd(p, PATH_MAX);
	old_prompt = ms->prompt;
	new_prompt = ft_strjoin_all(6, RD"["RST, e, RD"] ["RST, p, RD"] minishell"RST, "$ ");
	free(old_prompt);
	ms->prompt = new_prompt;
}

/**
 * @brief  Read a line of input using readline and handle the prompt display.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return int
 **        0 on successful input
 **        1 if the input is NULL (EOF or Ctrl-D)
 */
int	ft_readline(t_minishell *ms)
{
	// ft_create_prompt(ms);
	ms->swap_output_redirects = false;
	ms->swap_input_redirects = false;
	ms->input = readline(ms->prompt);
	if (ms->input == NULL)
	{
		// write(STDOUT_FILENO, "exit\n", 5);
		return (1) ;
	}
	if (ms->input)
		add_history(ms->input);
	return (0);
}

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
 * @brief  Tokenize the user input and check for unclosed quotes.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return int
 **        0 on success
 **        1 if there are unclosed quotes or other errors
 */
int	ft_handle_and_tokenize_input(t_minishell *ms)
{
	
	if (ft_check_quotes(ms->input))
		return (1); // erro por falta de fechar aspas!
	ms->n_args = ft_count_args(ms->input);
	if (ms->n_args == -1)
		return (1);
	ms->tokens = ft_tokenize_input(ms->input, ms->n_args, 0, 0);
	ft_revalue_TKN_VAR(ms);
	return (0);
}

/**
 * @brief  Process the user input and execute commands.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return int
 **        0 on successful execution
 **        1 on error
 */
int	ft_process_input_and_execute(t_minishell *ms)
{
	if (ft_handle_and_tokenize_input(ms))
	{
		if (ft_exit_code(ms) == 2)
			return (1);
		else
			return (ft_putstr_and_return("minishell: unclosed quotes\n", 1));
	}
 	ms->ast_root = ft_parse_ast(ms->tokens);
	ft_find_stuck_cats(ms, ms->ast_root);
	// print_ast(ms->ast_root, 5); // Para testar a estrutura da AST, se necessário
	if (ms->ast_root)
	{
		ms->status = ft_execute_ast(ms->ast_root, ms);
		if (dup2(ms->save_stdin, STDIN_FILENO) == -1
			|| dup2(ms->save_stdout, STDOUT_FILENO) == -1)
		{
			ft_close_stdin_stdout(ms);
			return(ft_perror("dup2", 1));
		}
		if (ms->in_pipe == true)
			ms->in_pipe = false;
		else
			ft_close_stdin_stdout(ms);
		ft_clean_stuck_cats(ms);
	}
	return (0);
}

/**
 * @brief  Handle stuck "cat" commands waiting for input.
 * 
 * @param  ms  Pointer to the minishell structure.
 * @return void
 */
void ft_clean_stuck_cats(t_minishell *ms)
{
	char c;

	if (!ms->c_stuck_cats)
		return ;
	while (ms->c_stuck_cats)
	{
		while (read(STDIN_FILENO, &c, 1) > 0)
    	{
       		if (c == '\n') // Enter pressionado
				ms->c_stuck_cats--;
			if (ms->c_stuck_cats == 0)
				break ;
		}
	}
	ft_set_exit_code(ms, 0);
	return ;
}

/**
 * @brief  Find and count stuck "cat" commands in the AST.
 * 
 * @param  ms    Pointer to the minishell structure.
 * @param  node  Current node in the AST.
 * @return void
 */
void ft_find_stuck_cats(t_minishell *ms, t_node *node)
{
	t_node *current;
	
	current = node;
	if (!current)
		return;
	if (current->token->type == TKN_CMD)
	{
		if (current->cmd_ready[1] == NULL
		&& (!ft_strcmp(current->cmd_ready[0], "cat")
		|| !ft_strcmp(current->cmd_ready[0], "/bin/cat"))
		&& current->prev && current->prev->token->type == TKN_PIPE 
		&& (current->prev->left == current || (current->prev->prev
		&& current->prev->prev->token->type == TKN_PIPE
		&& current->prev->right == current)))
			ms->c_stuck_cats++;
	}
	if (!current->left && !current->right)
		return ;
	ft_find_stuck_cats(ms, current->left);
	if (!ms->c_stuck_cats)
		return ;
	ft_find_stuck_cats(ms, current->right);	
}
