/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alborghi <alborghi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:02:07 by alborghi          #+#    #+#             */
/*   Updated: 2025/03/21 16:19:39 by alborghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_child(int *fd, t_data *data)
{
	close(fd[0]);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
	{
		close(fd[1]);
		ft_exit(data, 1);
	}
	close(fd[1]);
	if (call_function(data) == -1)
	{
		ft_exit(data, 130);
	}
	ft_exit(data, data->out);
}

void	pipe_parent(int *fd, t_data *data)
{
	close(fd[1]);
	if (dup2(fd[0], STDIN_FILENO) == -1)
	{
		close(fd[0]);
		ft_exit(data, 1);
	}
	if (!is_builtin(data->cmds->cmd))
		close(fd[0]);
	else
		data->fds = add_int_list(data->fds, fd[0]);
	data->cmds = data->cmds->next;
	exec_cmd(data);
	if (!check_pipe(data->cmds))
	{
		close(fd[0]);
	}
}

void	do_pipe(t_data *data)
{
	int	fd[2];
	int	pid;

	if (pipe(fd) == -1)
		return (perror("pipe"));
	pid = fork();
	if (pid == -1)
		return (perror("fork"));
	data->pids = add_int_list(data->pids, pid);
	if (pid == 0)
		pipe_child(fd, data);
	else
		pipe_parent(fd, data);
}

void	exec_cmd(t_data *data)
{
	int	pid;

	if (check_pipe(data->cmds))
		do_pipe(data);
	else
	{
		if (!data->head->next)
			call_function(data);
		else
		{
			pid = fork();
			if (pid == -1)
				return (perror("fork"));
			data->pids = add_int_list(data->pids, pid);
			if (pid == 0)
			{
				if (call_function(data) == -1)
					ft_exit(data, 130);
				ft_exit(data, data->out);
			}
		}
		reset_std(data);
	}
}
