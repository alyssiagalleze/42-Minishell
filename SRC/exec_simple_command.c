/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 14:00:42 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/09 12:13:00 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	here_doc_input(int i, t_exec *exec_list)
// {
// 	int		pipefds[2];
// 	char	*line;
	
// 	if (pipe(pipefds) == -1)
// 		return (print_err("heredoc pipe : pipe creation failed.\n", NULL, NULL, NULL), -1);

// 	line = NULL;
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line)
// 		{
// 			print_err(PROMPT, "warning: here-document (at line .. )delimited by end-of-file (wanted '", exec_list->command->redir_in[i], "')\n" );
// 			break;
// 		}
// 		if (str_ncmp(line, exec_list->command->redir_in[i], ft_strlen(exec_list->command->redir_in[i]) - 3, FALSE) == TRUE)
// 			break;
// 		write(pipefds[1], line, ft_strlen(line));
// 		write(pipefds[1], "\n", 1);
// 		free(line);
// 	}
// 	close(pipefds[1]);
// 	return (pipefds[0]);
// }

// int	open_fd_in(int *i, t_exec *exec_list)
// {
// 	int fd;
	
// 	if (exec_list->command->redir_kind == IN)
// 	{
// 		fd = open(exec_list->command->redir[i], O_RDONLY);
// 		if (fd == -1)
// 			return (print_err(PROMPT, exec_list->command->redir_in[i], ": ", "No such file of directory"), -1);
// 	}
// 	else
// 	{
// 		fd = here_doc_input(*i, exec_list);
		
// 	}
// 	return (fd);
// }

// int	open_fds(t_exec *exec_list)
// {
// 	int	i;
// 	int	fd;
	
// 	i = 0;
// 	while (exec_list->command->redir[i])
// 	{
// 		if (exec_list->command->redir_kind[i] == IN || exec_list->command->redir_kind[i] == HDOC)
// 			open_fd_in(&i, exec_list);
// 		if (exec_list->command->redir_kind[i] == OUT || exec_list->command->redir_kind[i] == OUT_APP)
		
// 		if (exec_list->command->is_redir_in_heredoc[i])
// 		{
// 			fd = here_doc_input(exec_list, i);	
// 			if (fd == -1)
// 				return 0;
// 		}
// 		else
// 		{
// 			fd = open(exec_list->command->redir_in[i], O_RDONLY);
// 			if (fd == -1)
// 				return (print_err(PROMPT, exec_list->command->redir_in[i], ": ", "No such file of directory"), -1);
// 		}
// 		i++;
// 	}
// 	return (fd);		
// }

// int	open_fd_out(t_exec *exec_list)
// {
// 	int fd;
// 	int i;

// 	i = 0;
// 	while (exec_list->command->redir_out[i])
// 	{
// 		if (exec_list->command->is_redir_out_append[i])
// 		{
// 			fd = open(exec_list->command->redir_out[i], O_RDWR | O_APPEND | O_CREAT, 0777);
// 			if (fd == -1)
// 				return (print_err(PROMPT, exec_list->command->redir_out[i], ": ", "No such file of directory\n"), -1);
// 		}	
// 		else 
// 		{
// 			fd = open(exec_list->command->redir_out[i], O_RDWR | O_TRUNC | O_CREAT, 0777);
// 			if (fd == -1)
// 				return (/* print_err(PROMPT, exec_list->command->redir_out[i], ": ", "No such file of directory\n"), */perror(exec_list->command->redir_out[i]), -1);
// 		}
// 		i++;
// 	}
// 	return (fd);
// }

// int	redirect_fds(t_exec *exec_list)
// {
// 	int	fd_in;
// 	int	fd_out;

// 	// fd_in = STDIN_FILENO;
// 	// fd_out = STDOUT_FILENO;
// 	if (exec_list->command->redir_in[0])
// 	{
// 		// printf("je redir in vers : %s\n", exec_list->command->redir_in[0]);
// 		fd_in = open_fd_in(exec_list);
// 		if (fd_in == -1)
// 			return (errno);
// 		if (dup2(fd_in, STDIN_FILENO) == -1)
// 			return (perror("dup in"), errno);
// 		close(fd_in);
// 	}
// 	if (exec_list->command->redir_out[0])
// 	{
// 		// printf("je redir out vers : %s\n", exec_list->command->redir_out[0]);
// 		fd_out = open_fd_out(exec_list);
// 		if (fd_out == -1)
// 			return (errno);
// 		if (dup2(fd_out, STDOUT_FILENO) == -1)
// 			return (perror("dup out"), errno);
// 		close(fd_out);
// 	}
// 	// else if (exec_list->next->is_command)
// 	// {
// 	// 	if (pipe(pipefds) == -1)
// 	// 		return (-1);
// 	// 	dup2(pipefds[1], STDOUT_FILENO);
// 	// 	dup2(pipefds[0], STDIN_FILENO);
// 	// }
// 	return (0);
// }

char	**get_paths(char **env_tab)
{
	char	**path_tab;
	int		i;
	
	i = 0;
	while (env_tab[i])
	{
		if (str_ncmp(env_tab[i], "PATH", 3, FALSE))
		{
			path_tab = ft_split(env_tab[i], ':');
			if (!path_tab)
				return (NULL);
		}
		i++;
	}
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

char	*set_command_path(t_exec *exec_list, char **env)
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
	return (NULL);
}

// int	is_builtin(t_exec *exec_list)
// {
// 	if (str_cmp(exec_list->command->argv[0], "echo", FALSE))
// 		return (TRUE);
// 	else if (str_cmp(exec_list->command->argv[0], "cd", FALSE))
// 		return (TRUE);
// 	else if (str_cmp(exec_list->command->argv[0], "pwd", FALSE))
// 		return (TRUE);
// 	else if (str_cmp(exec_list->command->argv[0], "export", FALSE))
// 		return (TRUE);
// 	else if (str_cmp(exec_list->command->argv[0], "unset", FALSE))
// 		return (TRUE);
// 	else if (str_cmp(exec_list->command->argv[0], "env", FALSE))
// 		return (TRUE);
// 	else if (str_cmp(exec_list->command->argv[0], "exit", FALSE))
// 		return (TRUE);
// 	else
// 		return (FALSE);
// }

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

char	*build_env_variable(t_env *node)
{
	char	*var;
	int		len;
	int		i;
	int		j;
	
	len = ft_strlen(node->var_name) + ft_strlen(node->var_value) + 2;
	var = malloc(len);
	if (!var)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len && node->var_name[j])
		var[i++] = node->var_name[j++];
	var[i] = '=';
	i++;
	j = 0;
	while (i < len && node->var_value[j])
		var[i++] = node->var_value[j++];
	var[i] = '\0';
	return (var);
}

char	**transfer_env(t_env **env)
{
	t_env	*current;
	char	**my_env;
	int		lst_len;
	int		i;
	
	lst_len = ft_lstsize(*env);
	i = 0;
	my_env = malloc((lst_len + 1)* (sizeof(char *)));
	if (!my_env)
		return (NULL);
	current = *env;
	while (current && i < lst_len)
	{
		my_env[i] = build_env_variable(current);
		if (!my_env[i])
			return (NULL);
		current = current->next;
		i++;
	}
	my_env[i] = NULL;
	return (my_env);
}

// int exec_pipeline(t_exec *exec_list, t_pid_list **pids, t_env **env, int *prev_fd)
// {
// 	int pipefds[2] = {-1, -1};
// 	pid_t pid;
// 	char *path;
// 	int status = 0;
// 	char	**my_env;
	
// 	if (!exec_list || exec_list->is_subshell || !exec_list->is_command || !exec_list->command)
//         return (print_err(PROMPT, "internal: exec_pipeline called for non-command node\n", NULL, NULL), 1);
// 	exec_list->command->prev_fd = *prev_fd;
// 	my_env = transfer_env(env);
// 	if (!my_env)
// 		return (print_err(PROMPT, ": malloc: ", "environment transfer failed.", NULL), 2);
// 	if (exec_list->next)
// 	{
// 		if (pipe(pipefds) == -1)
// 			return (perror("pipe"), 1);
// 	}

// 	if (!is_builtin(exec_list) && exec_list->is_subshell == FALSE)
// 	{
// 		path = set_command_path(exec_list, my_env);
// 		if (!path)
// 		{
// 			print_err(exec_list->command->argv[0], ": command not found\n", NULL, NULL);
// 			if (pipefds[0] != -1) close(pipefds[0]);
// 			if (pipefds[1] != -1) close(pipefds[1]);
// 			return (127);
// 		}
// 	}

// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		if (pipefds[0] != -1) close(pipefds[0]);
// 		if (pipefds[1] != -1) close(pipefds[1]);
// 		return (perror("fork"), 1);
// 	}

// 	if (pid == 0)
// 	{
// 		if (*prev_fd != -1)
// 		{
// 			if (dup2(*prev_fd, STDIN_FILENO) == -1)
// 				exit(1);
// 			close(*prev_fd);
// 		}

// 		if (exec_list->next)
// 		{
// 			if (dup2(pipefds[1], STDOUT_FILENO) == -1)
// 				exit(1);
// 			close(pipefds[1]);
// 			close(pipefds[0]);
// 		}

// 		// if (exec_list->is_subshell == TRUE)
// 		// 	exit(0);

// 		if (is_builtin(exec_list))
// 		{
// 			status = built_in_exec(exec_list, env);
// 			exit(status);
// 		}
// 		else
// 		{
// 			status = execve(path, exec_list->command->argv, my_env);
// 			perror(exec_list->command->argv[0]);
// 			exit(85);
// 		}
// 	}
	
// 	pid_add_back(pids, pid);

// 	if (*prev_fd != -1)
// 	{
// 		close(*prev_fd);
// 		// *prev_fd = -1;
// 	}

// 	if (exec_list->next)
// 	{
// 		if (pipefds[1] != -1)
// 			close(pipefds[1]);
// 		*prev_fd = pipefds[0];
// 	}
// 	else
// 	{
// 		if (pipefds[0] != -1) close(pipefds[0]);
// 		if (pipefds[1] != -1) close(pipefds[1]);
// 		// *prev_fd = -1;
// 	}

// 	return (0);
// }

int prepare_env_and_pipe(t_exec *exec_list, t_env **env, char ***my_env, int pipefds[2])
{
    if (!exec_list || !env)
        return (print_err(PROMPT, ": internal: prepare_env_and_pipe bad args\n", NULL, NULL), 1);
    *my_env = transfer_env(env);
    if (!*my_env)
        return (print_err(PROMPT, ": malloc: ", "environment transfer failed.", NULL), 2);
    if (exec_list->next)
    {
        if (pipe(pipefds) == -1)
            return (perror("pipe"), 1);
    }
    return (0);
}

char *get_path_for_command(t_exec *exec_list, char **my_env, int pipefds[2])
{
    char *path;

    path = NULL;
    if (!is_builtin(exec_list) && exec_list->is_subshell == FALSE)
    {
        path = set_command_path(exec_list, my_env);
        if (!path)
        {
            print_err(exec_list->command->argv[0], ": command not found\n", NULL, NULL);
            if (pipefds[0] != -1) close(pipefds[0]);
            if (pipefds[1] != -1) close(pipefds[1]);
            return (NULL);
        }
    }
    return (path);
}

void free_env_array(char **envp)
{
    int i;

    i = 0;
    if (!envp)
        return ;
    while (envp[i])
        free(envp[i++]);
    free(envp);
}

void child_exec(t_exec *exec_list, char *path, t_env **env, int pipefds[2])
{
    int		prev;
    int		status;
    char	**my_env;

    prev = exec_list->command->prev_fd;
    if (prev != -1)
    {
        if (dup2(prev, STDIN_FILENO) == -1)
            _exit(1);
        close(prev);
    }
    if (exec_list->next)
    {
        if (dup2(pipefds[1], STDOUT_FILENO) == -1)
            _exit(1);
        close(pipefds[1]);
        close(pipefds[0]);
    }
    if (is_builtin(exec_list))
    {
        status = built_in_exec(exec_list, env);
        _exit(status);
    }
    my_env = transfer_env(env);
    status = execve(path, exec_list->command->argv, my_env);
    perror(exec_list->command->argv[0]);
    free_env_array(my_env);
    _exit(85);
}

int	handle_fork_error(int pipefds[2], char **my_env)
{
    if (pipefds[0] != -1)
        close(pipefds[0]);
    if (pipefds[1] != -1)
        close(pipefds[1]);
    free_env_array(my_env);
    perror("fork");
    return (1);
}

void	parent_after_fork(t_pid_list **pids, pid_t pid, int *prev_fd, int pipefds[2])
{
    pid_add_back(pids, pid);
    if (*prev_fd != -1)
        close(*prev_fd);
    if (pipefds[1] != -1)
        close(pipefds[1]);
    *prev_fd = (pipefds[0] != -1) ? pipefds[0] : -1;
}

int exec_pipeline(t_exec *exec_list, t_pid_list **pids, t_env **env, int *prev_fd)
{
    int		pipefds[2] = {-1, -1};
    pid_t	pid;
    char	*path;
    char	**my_env;

    if (!exec_list || exec_list->is_subshell || !exec_list->is_command || !exec_list->command)
        return (print_err(PROMPT, "internal: exec_pipeline called for non-command node\n", NULL, NULL), 1);
    exec_list->command->prev_fd = *prev_fd;
    if (prepare_env_and_pipe(exec_list, env, &my_env, pipefds) != 0)
        return (1);
    path = get_path_for_command(exec_list, my_env, pipefds);
    if (!path && !is_builtin(exec_list))
        return (free_env_array(my_env), 127);
    pid = fork();
    if (pid == -1)
        return (handle_fork_error(pipefds, my_env));
    if (pid == 0)
        child_exec(exec_list, path, env, pipefds);
    parent_after_fork(pids, pid, prev_fd, pipefds);
    free_env_array(my_env);
    return (0);
}
