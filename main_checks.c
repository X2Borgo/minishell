/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_checks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alborghi <alborghi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:55:53 by alborghi          #+#    #+#             */
/*   Updated: 2025/03/24 08:47:50 by alborghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_signal;

int	check_is_folder(char *file)
{
	struct stat	buf;

	if (!file)
		return (0);
	if (stat(file, &buf) == 0)
	{
		if (S_ISDIR(buf.st_mode))
		{
			ft_printe("minishell: %s: is a directory\n", file);
			return (1);
		}
	}
	return (0);
}

int	check_files(t_cmd *cmds)
{
	int		i;

	while (cmds)
	{
		if (check_is_folder(cmds->file_a) == 1
			|| check_is_folder(cmds->file_o) == 1)
		{
			return (1);
		}
		i = 0;
		while (cmds->file_i && cmds->file_i[i])
		{
			if (check_is_folder(cmds->file_i[i]) == 1)
				return (1);
			i++;
		}
		cmds = cmds->next;
	}
	return (0);
}

int	parsing_checks(t_data *data)
{
	if (data->cmds == NULL)
	{
		data->status = 0;
		return (1);
	}
	if (data->status != 0)
	{
		data->status = 0;
		free_cmds(data->cmds);
		return (1);
	}
	if (check_files(data->cmds) == 1)
	{
		free_cmds(data->cmds);
		data->status = 1;
		return (1);
	}
	return (0);
}
