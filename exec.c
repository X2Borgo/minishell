/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alborghi <alborghi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 09:33:19 by alborghi          #+#    #+#             */
/*   Updated: 2025/03/24 08:48:50 by alborghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_put_env(t_env *env, int is_env)
{
	if (!env)
		return (0);
	while (env)
	{
		if (!(is_env == TRUE && env->is_env == FALSE))
			ft_printf("%s\n", env->var);
		env = env->next;
	}
	return (0);
}

int	open_last(char **file, int doi)
{
	int		fd;
	int		i;
	char	*tmp;

	if (!file || !file[0])
		return (-1);
	i = 0;
	while (file[i + 1])
	{
		fd = open(file[i], O_RDONLY);
		if (fd == -1)
			return (-1);
		close(fd);
		i++;
	}
	fd = open(file[i], O_RDONLY);
	if (fd == -1)
		return (-1);
	if (doi != 1)
		return (close(fd), 0);
	tmp = ft_strdup(file[i]);
	if (!tmp || dup_file(tmp, 0, O_RDONLY) == -1)
		return (free(tmp), -1);
	return (free(tmp), close(fd), 0);
}

int	handle_files(t_cmd *cmd, t_data *data)
{
	if (cmd->here_file)
	{
		if (read_heredoc(cmd->here_file) == 1)
			return (-1);
		if (g_signal == 2)
			return (g_signal = 0, data->out = 130, -1);
	}
	if (cmd->file_i)
	{
		if (open_last(cmd->file_i, cmd->doi) == -1)
			return (perror("minishell"), -1);
	}
	if (cmd->file_o)
	{
		if (dup_file(cmd->file_o, 1, O_CREAT | O_WRONLY | O_TRUNC) == -1)
			return (cmd->file_o = NULL, perror("minishell"), -1);
	}
	else if (cmd->file_a)
	{
		if (dup_file(cmd->file_a, 1, O_CREAT | O_WRONLY | O_APPEND) == -1)
			return (cmd->file_a = NULL, perror("minishell"), -1);
	}
	return (0);
}

//TODO: do check on data->status and if value is -1 exit program with status 1
int	call_function(t_data *data)
{
	if (data->cmds->out_error == 1)
		return (data->out = 1,
			ft_printe("minishell: : No such file or directory\n"), 0);
	if (handle_files(data->cmds, data) == -1)
		return (-1);
	if (!data->cmds->cmd || !data->cmds->cmd[0])
		return (0);
	if (ft_strncmp(data->cmds->cmd, "echo", 5) == 0)
		data->out = exec_echo(data->cmds->args);
	else if (ft_strncmp(data->cmds->cmd, "cd", 3) == 0)
		data->out = exec_cd(data);
	else if (ft_strncmp(data->cmds->cmd, "pwd", 4) == 0)
		data->out = print_pwd(data->pwd);
	else if (ft_strncmp(data->cmds->cmd, "export", 7) == 0)
		data->out = exec_export(data->cmds, data->env);
	else if (ft_strncmp(data->cmds->cmd, "unset", 6) == 0)
		data->out = exec_unset(data);
	else if (ft_strncmp(data->cmds->cmd, "env", 4) == 0)
		data->out = ft_put_env(data->env, TRUE);
	else if (ft_strncmp(data->cmds->cmd, "exit", 5) == 0)
		ft_exit_builtin(data);
	else
		data->out = exec_execve(data);
	return (0);
}

int	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}
