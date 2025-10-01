/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 14:00:42 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/01 16:54:59 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

int	here_doc_input(t_exec *exec_list, int i)
{
	int		pipefds[2];
	char	*line;
	
	if (pipe(pipefds) == -1)
		return (print_err("heredoc pipe : pipe creation failed.\n", NULL, NULL, NULL), -1);

	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			print_err(PROMPT, "warning: here-document (at line .. )delimited by end-of-file (wanted '", exec_list->command->redir_in[i], "')\n" );
			break;
		}
		if (str_ncmp(line, exec_list->command->redir_in[i], ft_strlen(exec_list->command->redir_in[i]) - 3, FALSE) == TRUE)
			break;
		write(pipefds[1], line, ft_strlen(line));
		write(pipefds[1], "\n", 1);
		free(line);
	}
	close(pipefds[1]);
	return (pipefds[0]);
}

int	open_fd_in(t_exec *exec_list)
{
	int	i;
	int	fd;
	
	i = 0;
	while (exec_list->command->redir_in[i])
	{
		if (exec_list->command->is_redir_in_heredoc[i])
		{
			fd = here_doc_input(exec_list, i);	
			if (fd == -1)
				return 0;
		}
		else
		{
			fd = open(exec_list->command->redir_in[i], O_RDONLY);
			if (fd == -1)
				return (print_err(PROMPT, exec_list->command->redir_in[i], ": ", "No such file of directory"), -1);
		}
		i++;
	}
	return (fd);		
}

int	open_fd_out(t_exec *exec_list)
{
	int fd;
	int i;

	i = 0;
	while (exec_list->command->redir_out[i])
	{
		if (exec_list->command->is_redir_out_append[i])
		{
			fd = open(exec_list->command->redir_out[i], O_RDWR | O_APPEND | O_CREAT, 0777);
			if (fd == -1)
				return (print_err(PROMPT, exec_list->command->redir_out[i], ": ", "No such file of directory\n"), -1);
		}	
		else 
		{
			fd = open(exec_list->command->redir_out[i], O_RDWR | O_TRUNC | O_CREAT, 0777);
			if (fd == -1)
				return (/* print_err(PROMPT, exec_list->command->redir_out[i], ": ", "No such file of directory\n"), */perror(exec_list->command->redir_out[i]), -1);
		}
		i++;
	}
	return (fd);
}

int	redirect_fds(t_exec *exec_list)
{
	int	fd_in;
	int	fd_out;

	// fd_in = STDIN_FILENO;
	// fd_out = STDOUT_FILENO;
	if (exec_list->command->redir_in[0])
	{
		// printf("je redir in vers : %s\n", exec_list->command->redir_in[0]);
		fd_in = open_fd_in(exec_list);
		if (fd_in == -1)
			return (errno);
		if (dup2(fd_in, STDIN_FILENO) == -1)
			return (perror("dup in"), errno);
		close(fd_in);
	}
	if (exec_list->command->redir_out[0])
	{
		// printf("je redir out vers : %s\n", exec_list->command->redir_out[0]);
		fd_out = open_fd_out(exec_list);
		if (fd_out == -1)
			return (errno);
		if (dup2(fd_out, STDOUT_FILENO) == -1)
			return (perror("dup out"), errno);
		close(fd_out);
	}
	// else if (exec_list->next->is_command)
	// {
	// 	if (pipe(pipefds) == -1)
	// 		return (-1);
	// 	dup2(pipefds[1], STDOUT_FILENO);
	// 	dup2(pipefds[0], STDIN_FILENO);
	// }
	return (0);
}

char	**get_paths(t_env **env)
{
	t_env	*current;
	char	**path_tab;
	
	current = *env;
	path_tab = NULL;
	while (current)
	{
		if (str_ncmp(current->var_name, "PATH", 3, FALSE))
			path_tab = ft_split(current->var_value, ':');
		current = current->next;
	}
	if (!path_tab)
		return (NULL);
	return (path_tab);
}

char	*append_exec_file(char *cmd_name, char *path)
{
	char	*tmp;
	char	*cmd_path;
	
	tmp = NULL;
	tmp = ft_strjoin(path, "/");
	if (!tmp)
		return (NULL);
	cmd_path = ft_strjoin(tmp, cmd_name);
	if (!cmd_path)
	{
		free(tmp);
		return (NULL);
	}
	free(tmp);
	return (cmd_path);
}

char	*set_command_path(t_exec *exec_list, t_env **env)
{
	int		i;
	char	**path_tab;
	char	*cmd_path;
	
	path_tab = get_paths(env);
	if (!path_tab)
		return (NULL);
	cmd_path = NULL;
	i = 0;
	while (path_tab[i])
	{
		cmd_path = append_exec_file(exec_list->command->argv[0], path_tab[i]);
		if (!access(cmd_path, F_OK | X_OK))
			return (exec_cleaner(path_tab, NULL), cmd_path);
		free(cmd_path);	
		i++;
	}
	exec_cleaner(path_tab, NULL);
	cmd_path = append_exec_file(exec_list->command->argv[0], ".");
	if (!access(cmd_path, F_OK | X_OK))
		return (cmd_path);
	free(cmd_path);
	return (perror(exec_list->command->argv[0]), NULL);
}

int	is_builtin(t_exec *exec_list)
{
	if (str_cmp(exec_list->command->argv[0], "echo", FALSE))
		return (TRUE);
	else if (str_cmp(exec_list->command->argv[0], "cd", FALSE))
		return (TRUE);
	else if (str_cmp(exec_list->command->argv[0], "pwd", FALSE))
		return (TRUE);
	else if (str_cmp(exec_list->command->argv[0], "export", FALSE))
		return (TRUE);
	else if (str_cmp(exec_list->command->argv[0], "unset", FALSE))
		return (TRUE);
	else if (str_cmp(exec_list->command->argv[0], "env", FALSE))
		return (TRUE);
	else if (str_cmp(exec_list->command->argv[0], "exit", FALSE))
		return (TRUE);
	else
		return (FALSE);
}

int	built_in_exec(t_exec *exec_list, t_env **env)
{
	int	exit_status;
	
	exit_status = 0;
	if (str_cmp(exec_list->command->argv[0], "echo", FALSE) == TRUE)
		exit_status = echo(exec_list->command->argv);
	else if (str_cmp(exec_list->command->argv[0], "cd", FALSE))
		exit_status = cd(exec_list->command->argv, env);
	else if (str_cmp(exec_list->command->argv[0], "pwd", FALSE))
		exit_status = pwd();
	else if (str_cmp(exec_list->command->argv[0], "export", FALSE))
		exit_status = export(exec_list->command->argv, env);
	else if (str_cmp(exec_list->command->argv[0], "unset", FALSE))
		exit_status = unset(exec_list->command->argv, env);
	else if (str_cmp(exec_list->command->argv[0], "env", FALSE))
		exit_status = print_env(env);
	return (exit_status);
}


int	fork_exec(t_exec *exec_list, t_env **env)
{
	pid_t	pid;
	char	*cmd_path;
	int		status;
	int		exit_status;
	
	cmd_path = set_command_path(exec_list, env);
	if (!cmd_path)
		return (print_err(exec_list->command->argv[0], ": command not found", NULL, NULL), 127);
	exit_status = 0;
	status = 0;
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (!pid)
	{
		if (execv(cmd_path, exec_list->command->argv) == -1)
			return (perror(exec_list->command->argv[0]), errno);
		return (status);
	}
	else
	{
		free(cmd_path);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
		return (exit_status);
	}
}

// int	write_in_pipe()
// {
	
// }

int	exec_command(t_exec *exec_list, t_env **env)
{
	int status;
	int	initialfds[2];
	
	status = 0;
	initialfds[0] = dup(STDIN_FILENO);
	initialfds[1] = dup(STDOUT_FILENO);
	if (initialfds[0] == -1 || initialfds[1] == -1)
		return (/* print_err("could not duplicate STDOUT\n", NULL, NULL, NULL), */perror("DUPSTDOUT"), 1);
	if (exec_list->command->redir_in[0] || exec_list->command->redir_out[0])
		status = redirect_fds(exec_list);
	if (!exec_list->command->argv[0])
		return (status);
	if (is_builtin(exec_list))
	{
		status = built_in_exec(exec_list, env);
		if (!exec_list->next->is_command)
		{
			if (dup2(initialfds[0], STDIN_FILENO) == -1 || dup2(initialfds[1], STDOUT_FILENO) == -1)
				return (print_err("could not restore STD fds\n", NULL, NULL, NULL), 1);
		}
	}
	else
	{
		status = fork_exec(exec_list, env);
		if (dup2(initialfds[0], STDIN_FILENO) == -1 || dup2(initialfds[1], STDOUT_FILENO) == -1)
			return (print_err("could not restore STD fds\n", NULL, NULL, NULL), 1);
	}
	return (status);
}


/* pid_t	execute_pipe(char *cmd, char **env)
{
	int		pipefds[2];
	pid_t	process_id;
	char	*path;

	path = get_path(env);
	if (path == NULL)
		exit_pipex(127, "PATH variable not found");
	if (pipe(pipefds) == -1)
		exit_pipex(2, "pipe");
	process_id = fork();
	if (process_id == -1)
		exit_pipex(2, "fork");
	if (!process_id)
	{
		if (dup2(pipefds[1], STDOUT_FILENO) == -1)
			exit_pipex(2, "dup2");
		close_fds();
		empty_command(cmd, path);
	}
	if (dup2(pipefds[0], STDIN_FILENO) == -1)
		exit_pipex(2, "dup2");
	close_fds();
	free(path);
	return (process_id);
}
 */
