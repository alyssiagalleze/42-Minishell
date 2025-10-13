/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 14:00:42 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/11 16:37:44 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_paths(char **env_tab)
{
	char	**path_tab;
	int		i;

	i = 0;
	while (env_tab[i])
	{
		if (str_ncmp(env_tab[i], "PATH=", 3, FALSE))
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
	free(tmp);
	if (!cmd_path)
		return (NULL);
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
	my_env = malloc((lst_len + 1) * (sizeof(char *)));
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

int	prepare_env_and_pipe(
	t_exec *exec_list, t_env **env, char ***my_env, int pipefds[2])
{
	pipefds[0] = -1;
	pipefds[1] = -1;
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

char	*get_path_for_command(t_exec *exec_list, char **my_env, int pipefds[2])
{
	char	*path;

	path = NULL;
	if (!is_builtin(exec_list) && exec_list->is_subshell == FALSE)
	{
		path = set_command_path(exec_list, my_env);
		if (!path)
		{
			print_err(exec_list->command->argv[0], ": command not found\n", NULL, NULL);
			if (pipefds[0] != -1)
				close(pipefds[0]);
			if (pipefds[1] != -1)
				close(pipefds[1]);
			return (NULL);
		}
	}
	return (path);
}

void	free_env_array(char **envp)
{
	int i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}

// TODO : Too many args
void child_exec(
	t_exec *exec_list, char *path, t_env **env, int pipefds[2], int prev_fd)
{
	int		status;
	char	**my_env;

	if (prev_fd != -1)
	{
		if (dup2(prev_fd, STDIN_FILENO) == -1)
			exit(1);
		close(prev_fd);
	}
	if (exec_list->next)
		if (dup2(pipefds[1], STDOUT_FILENO) == -1)
			exit(1);
	if (pipefds[0] != -1)
		close(pipefds[0]);
	if (pipefds[1] != -1)
		close(pipefds[1]);
	if (is_builtin(exec_list))
	{
		status = built_in_exec(exec_list, env);
		exit(status);
	}
	my_env = transfer_env(env);
	status = execve(path, exec_list->command->argv, my_env);
	perror(exec_list->command->argv[0]);
	free_env_array(my_env);
	exit(85); //TODO : pas sur du code retour
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

void	parent_after_fork(t_exec *exec_list, int *prev_fd, int pipefds[2])
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (exec_list->next)
	{
		*prev_fd = pipefds[0];
		if (pipefds[1] != -1)
			close(pipefds[1]);
	}
	else
	{
		if (pipefds[0] != -1)
			close(pipefds[0]);
		if (pipefds[1] != -1)
			close(pipefds[1]);
		*prev_fd = -1;
	}
}

int	exec_pipeline(t_exec *exec_list, t_pid_list **pids, t_env **env, int *prev_fd)
{
	int		pipefds[2];
	pid_t	pid;
	char	*path;
	char	**my_env;

	if (!exec_list || exec_list->is_subshell || !exec_list->is_command || !exec_list->command)
		return (print_err(PROMPT, "internal: exec_pipeline called for non-command node\n", NULL, NULL), 1);
	if (prepare_env_and_pipe(exec_list, env, &my_env, pipefds) != 0)
		return (1);
	path = get_path_for_command(exec_list, my_env, pipefds);
	if (!path && !is_builtin(exec_list))
		return (free_env_array(my_env), 127);
	pid = fork();
	if (pid == -1)
		return (handle_fork_error(pipefds, my_env));
	if (pid == 0)
		child_exec(exec_list, path, env, pipefds, *prev_fd);
	parent_after_fork(exec_list, prev_fd, pipefds);
	pid_add_back(pids, pid);
	free_env_array(my_env);
	free(path);
	return (0);
}
