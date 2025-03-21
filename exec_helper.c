/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alborghi <alborghi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 11:55:11 by alborghi          #+#    #+#             */
/*   Updated: 2025/03/21 11:56:09 by alborghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_pipe(t_cmd *cmds)
{
	if (!cmds || !cmds->next)
		return (0);
	return (1);
}

// 0644 = rw-r--r-- (user: read, write; group: read; others: read)
int	dup_file(char *file, int std, int mode)
{
	int	fd;

	fd = open(file, mode, 0644);
	if (fd == -1)
		return (free(file), -1);
	if (dup2(fd, std) == -1)
		return (free(file), -1);
	close(fd);
	return (0);
}

void	free_files(t_cmd *cmd)
{
	if (cmd->delimiter)
		ft_free_mat_char(cmd->delimiter);
	if (cmd->file_i)
		ft_free_mat_char(cmd->file_i);
	if (cmd->file_o)
		free(cmd->file_o);
	if (cmd->file_a)
		free(cmd->file_a);
}

int	reset_std(t_data *data)
{
	if (dup2(data->stdi, 0) == -1)
		return (-1);
	if (dup2(data->stdo, 1) == -1)
		return (-1);
	return (0);
}

int	print_pwd(char *pwd)
{
	ft_printf("%s\n", pwd);
	return (0);
}
