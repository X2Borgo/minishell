/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-sant <fde-sant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 09:51:57 by alborghi          #+#    #+#             */
/*   Updated: 2025/03/23 10:01:57 by fde-sant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_heredoc(t_cmd *cmd)
{
	if (!cmd || !cmd->cmd)
		return (0);
	while (cmd->cmd)
	{
		if (cmd->delimiter)
			return (1);
		cmd = cmd->next;
	}
	return (0);
}

int	skip_continue(char *line, t_cmd *cmd, int *i, int *j)
{
	if (!line || !line[0])
	{
		ft_printe("minishell: warning: here-document at line %d delimited\
				by end-of-file (wanted `%s')\n", j, cmd->delimiter[*i]);
		*j = 1;
		(*i)++;
		return (1);
	}
	if (ft_strncmp(line, cmd->delimiter[*i],
			ft_strlen(cmd->delimiter[*i]) + 1) == 0)
	{
		free(line);
		*j = 1;
		(*i)++;
		return (1);
	}
	return (0);
}

int	skip_heredoc(t_cmd *cmd)
{
	char	*line;
	int		i;
	int		j;

	i = 0;
	j = 1;
	while (cmd->delimiter[i + 1])
	{
		line = readline("> ");
		if (g_signal == SIGINT)
			return (free(line), 130);
		if (skip_continue(line, cmd, &i, &j) == 1)
			continue ;
		free(line);
		j++;
	}
	return (0);
}

//TODO: do this but in a fork
int	do_heredoc(t_data *data)
{
	int		i;
	t_cmd	*tmp;

	i = 0;
	tmp = data->cmds;
	signal(SIGINT, sig_here);
	while (tmp)
	{
		if (tmp->delimiter && tmp->delimiter[0])
		{
			if (skip_heredoc(tmp) == 130)
				return (130);
			if (save_heredoc(tmp, i++) != 0)
				return (g_signal + 128);
		}
		else
			tmp->here_file = NULL;
		tmp = tmp->next;
	}
	signal(SIGINT, new_prompt);
	return (0);
}

int	read_heredoc(char *file)
{
	int		fd;
	char	*line;
	int		pipe_fd[2];

	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (1);
	if (pipe(pipe_fd) == -1)
		return (1);
	line = get_next_line(fd);
	while (line)
	{
		ft_putstr_fd(line, pipe_fd[1]);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	close(pipe_fd[1]);
	dup2(pipe_fd[0], 0);
	close(pipe_fd[0]);
	return (0);
}
