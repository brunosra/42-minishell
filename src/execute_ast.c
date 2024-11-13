/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:54:54 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/11/13 02:47:52 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execute_ast(t_node *node);
int	ft_handle_output_redirect(t_node *node);
int	ft_handle_input_redirect(t_node *node);
int	ft_handle_pipe(t_node *node);
int	ft_execute_command(t_node *node);

int	ft_execute_ast(t_node *node)
{
	if (!node)
		return (1);
	if (node->token->type == TOKEN_OUTPUT_REDIRECT)
		return (ft_handle_output_redirect(node));
	else if (node->token->type == TOKEN_INPUT_REDIRECT)
		return (ft_handle_input_redirect(node));
	else if (node->token->type == TOKEN_OPERATOR)
		return (ft_handle_pipe(node));
	else if (node->token->type == TOKEN_COMMAND)
		return (ft_execute_command(node));
	return (0);
}

int	ft_handle_output_redirect(t_node *node)
{
	int	fd;
	int	save_stdout;  // Variável para salvar o stdout original
	int	status;

	save_stdout = dup(STDOUT_FILENO);
	if (save_stdout == -1)
	{
		perror("dup");
		return (1);
	}
	fd = open(node->right->token->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		close(save_stdout);  // Fecha o stdout salvo
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		close(save_stdout);
		return (1);
	}
	close(fd);
	status = ft_execute_ast(node->left);
	if (dup2(save_stdout, STDOUT_FILENO) == -1) // Restaura o stdout original
	{
		perror("dup2");
		close(save_stdout);
		return (1);
	}
	close(save_stdout);  // Fecha o descritor de arquivo do stdout original
	return (status);
}

int	ft_handle_input_redirect(t_node *node)
{
	int	fd;
	int	save_stdin;  // Variável para salvar o stdin original
	int	status;

	// Salva o stdin original
	save_stdin = dup(STDIN_FILENO);
	if (save_stdin == -1)
	{
		perror("dup");
		return (1);
	}
	fd = open(node->right->token->value, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		close(save_stdin);
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		close(save_stdin);
		return (1);
	}
	close(fd);
	status = ft_execute_ast(node->left);
	if (dup2(save_stdin, STDIN_FILENO) == -1) 	// Restaura o stdin original
	{
		perror("dup2");
		close(save_stdin);
		return (1);
	}
	close(save_stdin);  // Fecha o descritor de arquivo do stdin original
	return (status);
}

int	ft_handle_pipe(t_node *node)
{
	int	pipefd[2];
	int	pid;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		exit(ft_execute_ast(node->left)); // Executa o lado esquerdo do pipe
	}
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	return (ft_execute_ast(node->right)); // Executa o lado direito do pipe
}

int	ft_execute_command(t_node *node)
{
	int	pid;
	int status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		execvp(node->cmd_ready[0], node->cmd_ready);
		perror("execvp");
		exit(1);
	}
	else
	{
		waitpid(pid, &status, 0); // Espera o processo filho terminar VAI TER QUE ALTERAR!
		if (WIFEXITED(status)) // Diagnóstico de saída do filho
				printf("Comando terminou com status %d\n", WEXITSTATUS(status));
	}
	return (0);
}
