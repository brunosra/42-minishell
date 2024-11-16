/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:54:54 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/11/16 16:44:50 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execute_ast(t_node *node, t_minishell *ms);
int	ft_handle_output_redirect(t_node *node, t_minishell *ms);
int	ft_handle_input_redirect(t_node *node, t_minishell *ms);
int	ft_handle_pipe(t_node *node, t_minishell *ms);
int	ft_execute_command(t_node *node, t_minishell *ms);
int	ft_handle_heredoc(t_node *node, t_minishell *ms);

int	ft_execute_ast(t_node *node, t_minishell *ms)
{
	if (!node)
		return (1);
	if (node->token->type == TOKEN_OUTPUT_REDIRECT)
		return (ft_handle_output_redirect(node, ms));
	else if (node->token->type == TOKEN_INPUT_REDIRECT)
		return (ft_handle_input_redirect(node, ms));
	else if (node->token->type == TOKEN_HEREDOC)
		return (ft_handle_heredoc(node, ms));
	else if (node->token->type == TOKEN_OPERATOR)
		return (ft_handle_pipe(node, ms));
	else if (node->token->type == TOKEN_COMMAND)
		return (ft_execute_command(node, ms));
	return (0);
}

int	ft_handle_heredoc(t_node *node, t_minishell *ms)
{
	char	*input;
	char	*temp;
	char	*joined;
 	int		tty_fd;
	int		save_stdout;

	temp = NULL;
	joined = NULL;
	if (pipe(ms->pipefd) == -1)
	{
		perror("pipe");
		return (1);
	}
	if (!isatty(STDOUT_FILENO))
	{
		save_stdout = dup(STDOUT_FILENO);
		if (save_stdout == -1)
		{
			perror("dup");
			close(ms->pipefd[0]);
			close(ms->pipefd[1]);
			return (1);
		}
  		tty_fd = open("/dev/tty", O_WRONLY); ///dev/tty
		if (tty_fd == -1)
		{
			perror("open /dev/tty");
			close(ms->pipefd[0]);
			close(ms->pipefd[1]);
			close(save_stdout);
			return (1);
		}
		dup2(ms->save_stdout, STDOUT_FILENO);
 		close(tty_fd);
	}
	ms->pid = fork();
	if (ms->pid == -1)
	{
		perror("fork");
		close(ms->pipefd[0]);
		close(ms->pipefd[1]);
		if (save_stdout != -1)
			dup2(save_stdout, STDOUT_FILENO);
		return (1);
	}
	if (ms->pid == 0)
	{
		close(ms->pipefd[0]);
		while (1)
		{
			input = readline("> ");
			if (!input || ft_strcmp(input, node->right->token->value) == 0)
				break;
			if (temp)
				joined = ft_strjoin(temp, input);
			else
				joined = ft_strjoin("", input);
			if (temp)
				free(temp);
			temp = joined;
			joined = ft_strjoin(temp, "\n");
			free(temp);
			temp = joined;
			free(input);
		}
		if (temp)
		{
			write(ms->pipefd[1], temp, ft_strlen(temp));
			free(temp);
		}
		close(ms->pipefd[1]);
		exit(0);
	}
	close(ms->pipefd[1]);
	dup2(ms->pipefd[0], STDIN_FILENO);
	close(ms->pipefd[0]);
	waitpid(ms->pid, &ms->status, 0);
	if (save_stdout != -1)
	{
		dup2(save_stdout, STDOUT_FILENO);
		close(save_stdout);
	}
	return (ft_execute_ast(node->left, ms));
}
int	ft_handle_output_redirect(t_node *node, t_minishell *ms)
{
	int	fd;

	if (!ft_strcmp(node->token->value, ">"))
	{
		fd = open(node->right->token->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	else
	{
		fd = open(node->right->token->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	if (fd == -1)
	{
		perror("open");
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (ft_execute_ast(node->left, ms));
}

int	ft_handle_input_redirect(t_node *node, t_minishell *ms)
{
	int	fd;

	fd = open(node->right->token->value, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (ft_execute_ast(node->left, ms));
}

int	ft_handle_pipe(t_node *node, t_minishell *ms)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (1);
	}
	ms->pid = fork();
	if (ms->pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (ms->pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		exit(ft_execute_ast(node->left, ms));
	}
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	return (ft_execute_ast(node->right, ms));
}

int	ft_execute_command(t_node *node, t_minishell *ms)
{
	ms->pid = fork();
	if (ms->pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (ms->pid == 0)
	{
		execvp(node->cmd_ready[0], node->cmd_ready);
		perror("execvp");
		exit(1);
	}
	waitpid(ms->pid, &ms->status, 0);
	return (0);
}
