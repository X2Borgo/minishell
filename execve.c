/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-sant <fde-sant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:23:19 by alborghi          #+#    #+#             */
/*   Updated: 2025/03/24 08:22:41 by fde-sant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_execve(char *exec, char **argv, char **env)
{
	free(exec);
	ft_free_mat_char(argv);
	ft_free_mat_char(env);
}

void	execve_child(t_data *data, char *path, char **argv, char **env)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	execve(path, argv, env);
	ft_printe("minishell: %s: ", path);
	perror("");
	ft_printe("\n");
	free_execve(path, argv, env);
	ft_exit(data, 1);
}

// + 128 to get the signal number
int	execute_command(char *path, char **argv, char **env, t_data *data)
{
	pid_t	pid;
	int		status;

	(void)status;
	(void)pid;
	pid = fork();
	if (pid == -1)
		return (perror("fork"), free_execve(path, argv, env), -1);
	if (pid == 0)
		execve_child(data, path, argv, env);
	else
	{
		signal(SIGINT, sig_here);
		waitpid(pid, &status, 0);
		signal(SIGINT, new_prompt);
		free_execve(path, argv, env);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
			ft_printe("Quit (core dumped)\n");
		if (WIFSIGNALED(status))
			return (WTERMSIG(status) + 128);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		return (status);
	}
	return (0);
}

int	absoulte_path(t_data *data, char **argv, char **env)
{
	char	*exec;

	exec = ft_strdup(data->cmds->cmd);
	if (!exec)
		return (ft_printe("minishell: %s: malloc error\n", exec),
			free_execve(exec, argv, env), 1);
	if (access(exec, F_OK | X_OK) != 0)
		return (ft_printe("minishell: %s: ", exec), perror(""),
			free_execve(exec, argv, env), 1);
	return (execute_command(exec, argv, env, data));
}

// TODO: add check to argv and env 
int	exec_execve(t_data *data)
{
	char	*path;
	char	*exec;
	char	**argv;
	char	**env;

	if (!data->cmds->cmd || strncmp(data->cmds->cmd, "", 1) == 0)
		return (ft_printe("%s: command not found\n", data->cmds->cmd), 127);
	argv = get_args(data->cmds);
	env = env_to_mat(data->env);
	if (ft_strchr(data->cmds->cmd, '/') != NULL)
		return (absoulte_path(data, argv, env));
	path = get_env(data->env, "PATH");
	exec = find_path(data->cmds->cmd, path);
	if (!exec)
		return (ft_printe("%s: command not found\n", data->cmds->cmd),
			free_execve(exec, argv, env), 127);
	return (execute_command(exec, argv, env, data));
}
