/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:54:54 by tcosta-f          #+#    #+#             */
/*   Updated: 2024/12/10 09:03:10 by tcosta-f         ###   ########.fr       */
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
void	ft_swap_redirects_values(t_node *node, t_type type);
void	ft_remove_created_files(t_node *node);
void	ft_create_files(t_node *node);
int	ft_collect_heredocs(t_node *node, t_minishell *ms);
int	ft_handle_multiple_heredocs(t_node *node, t_minishell *ms);

int	ft_execute_ast(t_node *node, t_minishell *ms)
{
	if (!node || !ms->n_args)
		return (1);
 	if (node->token->type == TOKEN_EXCEPT)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
		ft_putstr_fd(node->token->value, STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
		ms->exit_code = 2;
		return (1);
	}
	if (node->token->type == TOKEN_OUTPUT_REDIRECT)
	{
		if (ms->swap_output_redirects == false)
		{
			ft_swap_redirects_values(node, TOKEN_OUTPUT_REDIRECT);
			ms->swap_output_redirects = true;
		}
		return (ft_handle_output_redirect(node, ms));
	}
	else if (node->token->type == TOKEN_INPUT_REDIRECT)
	{
		if (ms->swap_input_redirects == false)
		{
			ft_swap_redirects_values(node, TOKEN_INPUT_REDIRECT);
			ms->swap_input_redirects = true;
		}
		return (ft_handle_input_redirect(node, ms));
	}
 	else if (node->token->type == TOKEN_HEREDOC)
	{
		if (node->left && node->left->token->type == TOKEN_HEREDOC)
		{
			if (ft_collect_heredocs(node, ms))
				return (1);
			return (ft_handle_multiple_heredocs(node, ms));
		}
		return (ft_handle_heredoc(node, ms));
	}
 	else if (node->token->type == TOKEN_OPERATOR)
		return (ft_handle_pipe(node, ms));
	else if (node->token->type == TOKEN_BUILTIN)
		return (ft_execute_command(node, ms));
	else if (node->token->type == TOKEN_COMMAND)
		return (ft_execute_command(node, ms));
	return (0);
}

void	ft_swap_redirects_values(t_node *node, t_type type)
{
	t_node	*current;
	char	*temp_value;

	if (!node || node->token->type != type)
		return;
	current = node;
	while (current && current->left && current->left->token->type == type)
		current = current->left;
	if (current != node) // Troca os valores do último nó (mais profundo) com o atual
	{
		temp_value = current->right->token->value;
		current->right->token->value = node->right->token->value;
		node->right->token->value = temp_value;
	}
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
	ft_set_fork_signals();
	if (ms->pid == 0)
	{
		ft_set_heredoc_signals();
		close(ms->pipefd[0]);
		while (1)
		{
			input = readline("> ");
 			if (input == NULL) // Ctrl-D ou EOF
			{
				ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", STDERR_FILENO);
				ft_putstr_fd(node->right->token->value, STDERR_FILENO);
				write(STDERR_FILENO, "')\n", 3); 
				ms->exit_code = 0;
				break; // Contar heredocs, criar variavel para saber qdo tem de fechar exit 130
			} 
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
			ft_revalue_heredock_input(&temp, ms);
			write(ms->pipefd[1], temp, ft_strlen(temp));
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
	ft_set_main_signals();
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
	else
		node->file = true;
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
		ft_remove_created_files(node->prev);
		ft_create_files(node->left);
		ms->exit_code = 1; // Checar se existem outputs_redirects para a frente em que os ficheiros precisem de ser criados!!
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
	char *input;
	char *temp;

	temp = NULL;
	if (!node->right) // Caso de pipe sem lado direito
	{
		input = readline("> ");
		if (!input) // Ctrl-D
		{
			ft_putstr_fd("minishell: syntax error: unexpected end of file\n", STDERR_FILENO);
			ms->exit_code = 258; // Código típico de erro de sintaxe
			return (258);
		}
		temp = ft_strjoin(ms->input, " ");
		free(ms->input);
		ms->input = ft_strjoin(temp, input);
		free(input);
		free(temp);
		ft_free_tokens(ms->tokens);
		ft_free_ast(ms->ast_root);
		ms->in_pipe = true;
		return (ft_process_input_and_execute(ms));
	}
	
	else if (!node->left)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
		ms->exit_code = 2; // Código de erro para erro de sintaxe
		return (2);
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
	// printf("node token: %s\n", node->token->value);
	if (!ft_strcmp(node->token->value, "echo"))
		ms->exit_code = ft_builtin_echo(node->cmd_ready);
	if (!ft_strcmp(node->token->value, "exit"))
	 	ft_builtin_exit(node->cmd_ready, ms);
	if (!ft_strcmp(node->token->value, "env"))
		/* ms->exit_code = */ ft_builtin_env(node->cmd_ready, ms);
	// if (!ft_strcmp(node->token->value, "pwd"))
	// 	/* ms->exit_code =  */ft_builtin_pwd(ms);
/* 	if (!ft_strcmp(node->token->value, "cd"))
		ft_builtin_cd(ms); */
	return (ms->exit_code);
}

int	ft_execute_command(t_node *node, t_minishell *ms)
{
	int	valid;
	int sig;

	valid = -1;
	sig = 0;
	if (!node->cmd_ready[0] || node->cmd_ready[0][0] == '\0')
	{
		ft_putstr_fd(": command not found\n", STDERR_FILENO); // ou Command '' not found
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
		//ft_set_fork_signals();
		if (node->token->type == TOKEN_BUILTIN)
			exit(ft_exec_builtins(node, ms));
		if (node->cmd_ready[0][0] == '/' || 									// Caminho absoluto ou relativo
			(node->cmd_ready[0][0] == '.' && node->cmd_ready[0][1] == '/') || 
			!ft_strncmp(node->cmd_ready[0], "../", 3)) 
		{
			valid = ft_is_valid_file(node->cmd_ready[0], X_OK); // Verifica se o arquivo é válido
			if (valid != 0)
				exit(valid); // Retorna o erro correspondente (127 ou 126)
			execve(node->cmd_ready[0], node->cmd_ready, ms->env.envp); // Executa diretamente se for válido
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(node->cmd_ready[0], STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			exit(127);
		}
		if (ft_find_executable(ms, node->cmd_ready[0]) == 127)
		{
			ft_putstr_fd(node->cmd_ready[0], STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			exit(127);
		}
		if (node->cmd_ready[1] == NULL && 
		!ft_strcmp(node->cmd_ready[0], "cat") && node->prev->token->type == TOKEN_OPERATOR
		&& node->prev && (node->prev->left == node 
		|| (node->prev->prev && node->prev->prev->token->type == TOKEN_OPERATOR && node->prev->right == node)))
			exit(13);
		execve(ms->env.full_path, node->cmd_ready, ms->env.envp); // Executa o executável encontrado
		perror("execve");
		exit(127);
	}
	ft_set_fork_signals();
	waitpid(ms->pid, &ms->status, 0);
	if (WIFEXITED(ms->status)) // Processo terminou normalmente
		ms->exit_code = WEXITSTATUS(ms->status);
	else if (WIFSIGNALED(ms->status)) // Processo foi terminado por um sinal
	{
		sig = WTERMSIG(ms->status);
		ms->exit_code = 128 + sig;
		if (sig == SIGINT)
			write(STDERR_FILENO, "\n", 1);
	}
	else
		ms->exit_code = 1;
	ft_set_main_signals();
	if (ms->exit_code != 0 && node->prev && node->prev->token->type == TOKEN_INPUT_REDIRECT) // Remoção de arquivos criados caso o comando falhe
	{
		ft_remove_created_files(node->prev);
	}
	if (node->token->type == TOKEN_BUILTIN && !ft_strcmp(node->cmd_ready[0], "exit") && ms->exit_code != 1) // Finaliza o shell se for exit
	{
		ft_free_tokens(ms->tokens);
		ft_free_ast(ms->ast_root);
		free(ms->input);
		exit(ms->exit_code);
	}
	return (ms->exit_code);
}

void	ft_remove_created_files(t_node *node)
{
	if (!node)
		return ;
	if (node->token->type == TOKEN_OUTPUT_REDIRECT && node->right && node->right->token->value && node->file == true) // Se for um OUTPUT_REDIRECT, tenta remover o arquivo associado
	{
		if (unlink(node->right->token->value) == -1)
			perror("unlink");
	}
	if (node->prev && node->prev->right == node) // Se estivermos em um ramo direito, seguimos para o nó anterior diretamente
	{
		ft_remove_created_files(node->prev);
		return ;
	}
	if (node->prev && node->prev->right) // Se estivermos em um ramo esquerdo, verificamos o ramo direito do nó anterior
		ft_remove_created_files(node->prev->right);
	if (node->prev) // Continua a recursão para o nó anterior
		ft_remove_created_files(node->prev);
}

void	ft_create_files(t_node *node)
{
	int fd;

	if (!node)
		return;
	if (node->token->type == TOKEN_OUTPUT_REDIRECT && node->right && node->right->token->value && node->file == false)
	{
		if (ft_strcmp(node->token->value, ">>") == 0)
			fd = open(node->right->token->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(node->right->token->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd != -1)
			close(fd);
		else
			perror("open");
	}
	if (node->left && node->left->token->type == TOKEN_OUTPUT_REDIRECT) // Se o nó à esquerda também for um output redirect, continuar a verificação
		ft_create_files(node->left);
}

int	ft_find_executable(t_minishell *ms, char *cmd)
{
	int	i;

	i = 0;
	ms->env.env_paths = getenv("PATH");
	if (!ms->env.env_paths)
		return (127); // PATH não encontrado
	ms->env.paths = ft_split(ms->env.env_paths, ':');
	if (!ms->env.paths)
		return (127); // Falha ao dividir o PATH
	while (ms->env.paths[i])
	{
		ms->env.full_path = malloc(ft_strlen(ms->env.paths[i]) + ft_strlen(cmd) + 2);
		if (!ms->env.full_path)
		{
			ft_free_split(ms->env.paths);
			return (127);
		}
		ft_strcpy(ms->env.full_path, ms->env.paths[i]);
		ft_strcat(ms->env.full_path, "/");
		ft_strcat(ms->env.full_path, cmd);
		if (access(ms->env.full_path, X_OK) == 0)
		{
			ft_free_split(ms->env.paths);
			return (0); // Comando encontrado
		}
		free(ms->env.full_path);
		i++;
	}
	ft_free_split(ms->env.paths);
	return (127); // Comando não encontrado
}

int	ft_invalid_right_token_value(char *value)
{
	if (!value || 
		ft_strcmp(value, ">") == 0 || ft_strcmp(value, ">>") == 0 || 
		ft_strcmp(value, "<") == 0 || ft_strcmp(value, "<<") == 0 || 
		ft_strcmp(value, "|") == 0 || ft_strcmp(value, "&&") == 0 || 
		ft_strcmp(value, "||") == 0)
		return (1);
	return (0);
}

int	ft_is_valid_file(char *filepath, int mode)
{
	struct stat	file_stat;
	
	if (!filepath) // Verifica se o caminho é válido
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
		return (1);
	}
	if (stat(filepath, &file_stat) == -1) // Verifica se o arquivo existe
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(filepath, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		return (127); // Código de erro para arquivo inexistente
	}
	if (S_ISDIR(file_stat.st_mode)) 	// Verifica se é um diretório
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(filepath, STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		return (126); // Código de erro para diretório
	}
	if (mode == X_OK) // Verifica permissões de execução
	{
		if (access(filepath, X_OK) == -1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(filepath, STDERR_FILENO);
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			return (126); // Código de erro para permissão negada
		}
		return (0);
	}
	if (mode == O_RDONLY) 	// Verifica permissões de leitura
	{
		if (access(filepath, R_OK) == -1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(filepath, STDERR_FILENO);
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			return (126); // Código de erro para permissão negada
		}
		return (0);
	}
	if (mode == O_WRONLY || mode == (O_WRONLY | O_CREAT) || mode == (O_WRONLY | O_APPEND)) // Escrita
	{
		if (access(filepath, W_OK) == -1)
		{
			if (errno != ENOENT) // Arquivo existe, mas sem permissão de escrita
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(filepath, STDERR_FILENO);
				ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
				return (126);
			}
		}	
	}
	return (0); // Arquivo válido
}


int ft_handle_multiple_heredocs(t_node *node, t_minishell *ms)
{
    char *input;
    int save_stdout;
    int status;
    t_node *current;
    int i;
    char *new_temp;

    save_stdout = -1;
    ms->temp = NULL;
    i = 0;
    new_temp = NULL;
    if (!node || !node->heredoc_stops)
        return (1);
    if (!isatty(STDOUT_FILENO)) // Salvar STDOUT se necessário
    {
        save_stdout = dup(STDOUT_FILENO);
        if (save_stdout == -1)
        {
            perror("dup");
            return (1);
        }
    }
    while (node->heredoc_stops[i]) // Processar todos os heredocs
    {
        if (g_interrupt)
        {
            ms->exit_code = 130;
            break;
        }
        if (pipe(ms->pipefd) == -1) // Criar o pipe para o heredoc
        {
            perror("pipe");
            return (1);
        }
        ms->pid = fork();
        if (ms->pid == -1)
        {
            perror("fork");
            close(ms->pipefd[0]);
            close(ms->pipefd[1]);
            return (1);
        }
        ft_set_fork_signals();
        if (ms->pid == 0)
        {
            ft_set_heredoc_signals();
            close(ms->pipefd[0]);
            ms->temp = NULL;
            while (!g_interrupt)
            {
                input = readline("> ");
                if (g_interrupt)
                    exit(130);
                if (!input)
                {
                    ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", STDERR_FILENO);
                    ft_putstr_fd(node->heredoc_stops[i], STDERR_FILENO);
                    write(STDERR_FILENO, "')\n", 3);
                    break;
                }
                if (ft_strcmp(input, node->heredoc_stops[i]) == 0)
                    break;
                if (ms->temp == NULL)
                    ms->temp = ft_strdup(input);
                else
                {
                    new_temp = ft_strjoin_free(ms->temp, "\n", 1, 0);
                    ms->temp = ft_strjoin_free(new_temp, input, 1, 0);
                }
                free(input);
            }
            if (ms->temp && ms->c_multi_heredocs == i + 1)
            {
                new_temp = ft_strjoin_free(ms->temp, "\n", 1, 0);
				ft_revalue_heredock_input(&new_temp, ms);
                ft_putstr_fd(new_temp, ms->pipefd[1]);
                free(new_temp);
            }
            close(ms->pipefd[1]);
            exit(0);
        }
        close(ms->pipefd[1]);
        waitpid(ms->pid, &status, 0);
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
        {
            g_interrupt = 1;
            ms->exit_code = 130;
            close(ms->pipefd[0]);
            break;
        }
        if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
        {
            ms->exit_code = 130;
            close(ms->pipefd[0]);
            break;
        }
        i++;
    }
    if (!WIFSIGNALED(status) && WEXITSTATUS(status) != 130) // Após todos os heredocs, redirecionar o stdin do processo pai para o pipe
    {
        if (dup2(ms->pipefd[0], STDIN_FILENO) == -1 && !g_interrupt)
        {
            perror("dup2 aqui");
            close(ms->pipefd[0]);
            ms->exit_code = 1;
            return (1);
        }
    }
    close(ms->pipefd[0]);
    ft_set_main_signals();
    if (save_stdout != -1) // Restaurar STDOUT se necessário
    {
        if (dup2(save_stdout, STDOUT_FILENO) == -1)
        {
            perror("dup2 este");
            close(save_stdout);
            ms->exit_code = 1;
            return (1);
        }
        close(save_stdout);
    }
    current = node->left;
    while (current && current->token->type == TOKEN_HEREDOC)
    {
        t_node *temp_node = current;
        current = current->left;
        free(temp_node);
    }
    node->left = current;
    g_interrupt = 0;
    return (ft_execute_ast(node->left, ms));
}


int	ft_collect_heredocs(t_node *node, t_minishell *ms)
{
	t_node	*current;
	int		i;

	i = 0;
	ms->c_multi_heredocs = 0;
	if (!node || node->token->type != TOKEN_HEREDOC)
		return (0);
	
	// Contar heredocs consecutivos
	current = node;
	while (current && current->token->type == TOKEN_HEREDOC)
	{
		ms->c_multi_heredocs++;
		current = current->left;
	}

	// Alocar espaço para os stop tokens
	node->heredoc_stops = malloc(sizeof(char *) * (ms->c_multi_heredocs + 1));
	if (!node->heredoc_stops)
		return (0);

	// Preencher os stop tokens do último para o primeiro
	current = node;
	i = ms->c_multi_heredocs;
	while (current && --i >= 0)
	{
		if(!current->right)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
			//ft_putstr_fd(current->left->token->value, STDERR_FILENO);
			ft_putchar_fd(current->left->token->value[0], STDERR_FILENO);
			ft_putstr_fd("'\n", STDERR_FILENO);
			ms->exit_code = 2;
			return (1);
		}
		node->heredoc_stops[i] = current->right->token->value;
		current = current->left;
	}
	node->heredoc_stops[ms->c_multi_heredocs] = NULL; // Terminar com NULL
	return (0);
}

