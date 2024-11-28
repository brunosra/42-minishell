/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:54:54 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/11/28 20:42:30 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execute_ast(t_node *node, t_minishell *ms);
int	ft_handle_heredoc(t_node *node, t_minishell *ms);
int	ft_handle_output_redirect(t_node *node, t_minishell *ms);
int	ft_handle_input_redirect(t_node *node, t_minishell *ms);
int	ft_handle_pipe(t_node *node, t_minishell *ms);
int	ft_execute_command(t_node *node, t_minishell *ms);
int	ft_find_executable(t_minishell *ms, char *cmd);
int	ft_invalid_right_token_value(char *value);
int	ft_is_valid_file(char *filepath, int mode);

int	ft_execute_ast(t_node *node, t_minishell *ms)
{
	if (!node || !ms->n_args)
		return (1);
	if (node->token->type == TOKEN_OUTPUT_REDIRECT)
		return (ft_handle_output_redirect(node, ms));
	else if (node->token->type == TOKEN_INPUT_REDIRECT)
		return (ft_handle_input_redirect(node, ms));
	else if (node->token->type == TOKEN_HEREDOC)
		return (ft_handle_heredoc(node, ms));
	else if (node->token->type == TOKEN_OPERATOR)
		return (ft_handle_pipe(node, ms));
	else if (node->token->type == TOKEN_BUILTIN)
		return (ft_execute_command(node, ms));
	else if (node->token->type == TOKEN_COMMAND)
		return (ft_execute_command(node, ms));
	return (0);
}

int	ft_handle_heredoc(t_node *node, t_minishell *ms)
{
	char	*input;
	char	*temp;
	int		save_stdout;
	int		tty_fd;

	input = NULL;
	temp = NULL;
	save_stdout = -1;
	tty_fd = -1;
	if (!node->right)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
		ms->exit_code = 2;
		return (1);
	}
	if (pipe(ms->pipefd) == -1)
	{
		perror("pipe");
		ms->exit_code = 1;
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
			ms->exit_code = 1;
			return (1);
		}
		tty_fd = open("/dev/tty", O_WRONLY);
		if (tty_fd == -1)
		{
			perror("open /dev/tty");
			close(ms->pipefd[0]);
			close(ms->pipefd[1]);
			close(save_stdout);
			ms->exit_code = 1;
			return (1);
		}
		if (dup2(ms->save_stdout, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close(tty_fd);
			close(ms->pipefd[0]);
			close(ms->pipefd[1]);
			close(ms->save_stdout);
			close(save_stdout);
			ms->exit_code = 1;
			return (1);
		}
		close(tty_fd);
	}
	ms->pid = fork();
	if (ms->pid == -1)
	{
		perror("fork");
		close(ms->pipefd[0]);
		close(ms->pipefd[1]);
		ms->exit_code = 1;
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
			{
				temp = ft_strjoin_free(temp, input, 1, 0);
				temp = ft_strjoin_free(temp, "\n", 1, 0);
			}
			else
			{
				temp = ft_strdup(input);
				temp = ft_strjoin_free(temp, "\n", 1, 0);
			}
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
	if (dup2(ms->pipefd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(ms->pipefd[0]);
		ms->exit_code = 1;
		return (1);
	}
	close(ms->pipefd[0]);
	waitpid(ms->pid, &ms->status, 0);
	if (save_stdout != -1)
	{
		if (dup2(save_stdout, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close(save_stdout);
			ms->exit_code = 1;
			return (1);
		}
		close(save_stdout);
	}
	if (WIFEXITED(ms->status))
		ms->exit_code = WEXITSTATUS(ms->status);
	else
		ms->exit_code = 1;
	return (ft_execute_ast(node->left, ms));
}

int	ft_handle_output_redirect(t_node *node, t_minishell *ms)
{
	int	fd;

	if (!node->right) // Verifica se o token à direita é inválido
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
		ms->exit_code = 2;
		return (1);
	}
	else if (ft_invalid_right_token_value(node->right->token->value))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
		if (node->right->token->value)
			ft_putstr_fd(node->right->token->value, STDERR_FILENO);
		else
			ft_putstr_fd("newline", STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
		ms->exit_code = 2;
		return (1);
	}
/* 	if (ft_is_valid_file(node->right->token->value, O_WRONLY)) // Verifica se o arquivo é inválido// verificar se e um diretorio!!
	{
		ms->exit_code = 1;
		return (1);
	} */
	if (ft_strcmp(node->token->value, ">>") == 0) // Abre o arquivo para escrita ou append
		fd = open(node->right->token->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(node->right->token->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1) // Trata erros ao abrir o arquivo
	{
		perror("open");
		ms->exit_code = 1;
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1) // Redireciona STDOUT para o arquivo
	{
		perror("dup2");
		close(fd);
		ms->exit_code = 1;
		return (1);
	}
	close(fd);
	if (node->left)
		return (ft_execute_ast(node->left, ms));
	else
	{
		ms->exit_code = 0;
		return (0);
	}
}

int	ft_handle_input_redirect(t_node *node, t_minishell *ms)
{
	int	fd;

	if (!node->right) // Verifica se o token à direita é inválido
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
		ms->exit_code = 2;
		return (1);
	}
	if (ft_invalid_right_token_value(node->right->token->value)) 	// Verifica se o token à direita é inválido
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
		if (node->right->token->value)
			ft_putstr_fd(node->right->token->value, STDERR_FILENO);
		else
			ft_putstr_fd("newline", STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
		ms->exit_code = 2;
		return (1);
	}
	if (ft_is_valid_file(node->right->token->value, O_RDONLY)) 	// Verifica se o arquivo é inválido
	{
		ms->exit_code = 1;
		return (1);
	}
	fd = open(node->right->token->value, O_RDONLY); // Abre o arquivo para leitura
	if (fd == -1)
	{
		perror("open");
		ms->exit_code = 1;
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1) // Redireciona STDIN para o arquivo
	{
		perror("dup2");
		close(fd);
		ms->exit_code = 1;
		return (1);
	}
	close(fd);
	ms->exit_code = 0; // Redirecionamento bem-sucedido
	return (ft_execute_ast(node->left, ms));
}

int	ft_handle_pipe(t_node *node, t_minishell *ms)
{
	// if (!node->right)
	// 	return (ft_execute_ast(node->left, ms));
	if (ft_invalid_right_token_value(node->right->token->value) == 1)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
		if (node->right->token->value)
			ft_putstr_fd(node->right->token->value, STDERR_FILENO);
		else
			ft_putstr_fd("newline", STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
		ms->exit_code = 258;
		return (1);
	}
	if (pipe(ms->pipefd) == -1)
	{
		perror("pipe");
		ms->exit_code = 1;
		return (1);
	}
	ms->pid = fork();
	if (ms->pid == -1)
	{
		perror("fork");
		ms->exit_code = 1;
		return (1);
	}
	if (ms->pid == 0)
	{
		close(ms->pipefd[0]);
		if (dup2(ms->pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(1);
		}
		close(ms->pipefd[1]);
		exit(ft_execute_ast(node->left, ms));
	}
	close(ms->pipefd[1]);
	if (dup2(ms->pipefd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		ms->exit_code = 1;
		close(ms->pipefd[0]);
		return (1);
	}
	close(ms->pipefd[0]);
	ms->exit_code = 0; // Pipe bem-sucedido
	return (ft_execute_ast(node->right, ms));
}

int	ft_exec_builtins(t_node *node, t_minishell *ms)
{
	printf("node token: %s\n", node->token->value);
	if (!ft_strcmp(node->token->value, "echo"))
		ms->exit_code = ft_builtin_echo(node->cmd_ready);
	if (!ft_strcmp(node->token->value, "env"))
		ft_builtin_env(ms);
	// if (!ft_strcmp(node->token->value, "pwd"))
	// 	return (printf("builtin: pwd -->"), 1);
	// if (!ft_strcmp(node->token->value, "exit"))
	// 	return (printf("builtin: exit -->"), 1);
	// if (!ft_strcmp(node->token->value, "cd"))
	return (ms->exit_code);
}

int	ft_execute_command(t_node *node, t_minishell *ms)
{
	if (!node->cmd_ready[0] || node->cmd_ready[0][0] == '\0')
	{
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		ms->exit_code = 127;
		return (127); // Código de erro para "command not found"
	}
	ms->pid = fork();
	if (ms->pid == -1)
	{
		perror("fork");
		ms->exit_code = 1; // Código genérico para erro de fork
		return (1);
	}
	if (ms->pid == 0)
	{
		if (ft_check_builtins(node->token->value))
		{
			exit (ft_exec_builtins(node, ms)); // Executa builtins, caso seja válido
		}
		else
		{
			if (node->cmd_ready[0][0] == '/' || 
				(node->cmd_ready[0][0] == '.' && node->cmd_ready[0][1] == '/') || 
				!ft_strncmp(node->cmd_ready[0], "../", 3)) 
			{
				execve(node->cmd_ready[0], node->cmd_ready, ms->env.envp);
				perror("execve");
				exit(127);
			}
			if (ft_find_executable(ms, node->cmd_ready[0]) == 127)
			{
				ft_putstr_fd(node->cmd_ready[0], STDERR_FILENO);
				ft_putstr_fd(": command not found\n", STDERR_FILENO);
				exit(127);
			}
			execve(ms->env.full_path, node->cmd_ready, ms->env.envp);
			perror("execve");
			exit(127);
		}
		exit(1); // Falha inesperada pois nunca devera aqui chegar
	}
	waitpid(ms->pid, &ms->status, 0); // Processo pai: aguarda o processo filho
	if (WIFEXITED(ms->status)) // Processo terminou normalmente
		ms->exit_code = WEXITSTATUS(ms->status); // Captura o código de saída do filho
	else if (WIFSIGNALED(ms->status)) // Processo foi terminado por um sinal
		ms->exit_code = 128 + WTERMSIG(ms->status); // Código do sinal + 128
	else
		ms->exit_code = 1; // Caso inesperado, erro genérico
	return (ms->exit_code);
}

int	ft_find_executable(t_minishell *ms, char *cmd)
{
	int		i;

	i = 0;
	ms->env.env_paths = getenv("PATH");
	if (!ms->env.env_paths)
		return (1);
	ms->env.paths = ft_split(ms->env.env_paths, ':'); // Divide PATH em diretórios
	if (!ms->env.paths)
		return (1);
	while (ms->env.paths[i])
	{
		ms->env.full_path = malloc(ft_strlen(ms->env.paths[i]) + ft_strlen(cmd) + 2); // +2 para '/' e '\0'
		if (!ms->env.full_path)
		{
			ft_free_split(ms->env.paths);
			return (1);
		}
		ft_strcpy(ms->env.full_path, ms->env.paths[i]);
		ft_strcat(ms->env.full_path, "/");
		ft_strcat(ms->env.full_path, cmd);
		if (access(ms->env.full_path, X_OK) == 0) // Verifica se é executável
		{
			ft_free_split(ms->env.paths);
			return (0);
		}
		free(ms->env.full_path);
		i++;
	}
	ft_free_split(ms->env.paths);
	return (127);
}

int	ft_invalid_right_token_value(char *value)
{
	if (!value)
		return (1);
	if (ft_strcmp(value, ">") == 0)
		return (1);
	if (ft_strcmp(value, ">>") == 0)
		return (1);
	if (ft_strcmp(value, "<") == 0)
		return (1);
	if (ft_strcmp(value, "<<") == 0)
		return (1);
	if (ft_strcmp(value, "|") == 0)
		return (1);
	if (ft_strcmp(value, "&&") == 0)
		return (1);
	if (ft_strcmp(value, "||") == 0)
		return (1);
	return (0);
}

int	ft_is_valid_file(char *filepath, int mode)
{
	struct stat	file_stat;

	if (!filepath)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
		return (1);
	}
	if (stat(filepath, &file_stat) == -1) 	// Verifica se o arquivo existe

	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(filepath, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		return (1);
	}	
	if (S_ISDIR(file_stat.st_mode)) // Verifica se o caminho é um diretório
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(filepath, STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		return (1);
	}
	if (mode == O_RDONLY) // Leitura 	// Verifica permissões de acesso
	{
		if (access(filepath, R_OK) == -1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(filepath, STDERR_FILENO);
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			return (1);
		}
	}
	else if (mode == O_WRONLY || mode == (O_WRONLY | O_CREAT) || mode == (O_WRONLY | O_APPEND)) // Escrita
	{
		if (access(filepath, W_OK) == -1)
		{
			if (errno != ENOENT) // Arquivo existe, mas sem permissão de escrita
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(filepath, STDERR_FILENO);
				ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
				return (1);
			}
		}
	}
	return (0); // Arquivo válido
}
