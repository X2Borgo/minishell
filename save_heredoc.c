/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-sant <fde-sant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:33:18 by alborghi          #+#    #+#             */
/*   Updated: 2025/03/23 10:01:38 by fde-sant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	continue_checks(char *line, t_cmd *cmd, int *i, int *j)
{
	if (!line)
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

int	save_cycle(t_cmd *cmd, int *i, int *j, int fd)
{
	char	*line;

	line = readline("> ");
	if (g_signal != 0)
		return (g_signal + 128);
	if (continue_checks(line, cmd, i, j))
		return (0);
	line = ft_strtrim_free(line, "\n");
	ft_putendl_fd(line, fd);
	free(line);
	(*j)++;
	return (0);
}

int	save_heredoc(t_cmd *cmd, int n)
{
	int		i;
	int		fd;
	int		flag;
	int		j;

	cmd->here_file = ft_strjoin_free_2("tmp/.heredoc_", ft_itoa(n));
	fd = open(cmd->here_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		return (1);
	i = ft_char_mat_len(cmd->delimiter) - 1;
	flag = 1;
	j = 1;
	while (cmd->delimiter[i])
	{
		if (save_cycle(cmd, &i, &j, fd) != 0)
			return (g_signal + 128);
	}
	close(fd);
	return (0);
}
