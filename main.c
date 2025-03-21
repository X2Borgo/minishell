/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-sant <fde-sant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 18:06:42 by alborghi          #+#    #+#             */
/*   Updated: 2025/03/21 19:05:17 by fde-sant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal;

void	ft_readline(t_data *data)
{
	char	*line;
	char	*history;

	line = readline(MINI);
	if (line == NULL)
	{
		ft_printf("exit\n");
		ft_exit(data, data->out);
	}
	history = ft_strtrim(line, "\n ");
	add_history(history);
	write_history(data->history);
	free(history);
	data->cmds = parsing(line, data);
	ft_printf("##########%d\n", data->cmds->out_error);
	free(line);
}

void	ft_waitpids(t_data *data)
{
	int	status;

	if (data->pids)
	{
		status = 0;
		while (data->pids && waitpid(data->pids->n, &status, 0) >= 0)
		{
			if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
			{
				ft_printe("Quit (core dumped)\n");
				g_signal = 0;
				break ;
			}
			if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			{
				data->out = WEXITSTATUS(status);
			}
			data->pids = remove_node(data->pids);
		}
		if (WIFSIGNALED(status))
			data->out = (WTERMSIG(status) + 128);
		if (WIFEXITED(status))
			data->out = (WEXITSTATUS(status));
	}
}

void	reset_and_free(t_data *data)
{
	init_signals();
	reset_std(data);
	free_cmds(data->head);
	close_fds(data->fds);
	data->fds = NULL;
	data->head = NULL;
	data->cmds = NULL;
}

// ctrl + d -> EOF (get_next_line returns NULL) -> exit
int	main(int ac, char **av, char **env)
{
	char	*line;
	t_data	data;

	(void)ac;
	(void)av;
	if (init_data(&data, env) == 1)
		ft_exit(&data, 1);
	using_history();
	init_signals();
	line = NULL;
	while (1)
	{
		data.status = 0;
		g_signal = 0;
		ft_readline(&data);
		if (parsing_checks(&data) == 1)
			continue ;
		do_heredoc(&data);
		data.head = data.cmds;
		exec_cmd(&data);
		ft_waitpids(&data);
		reset_and_free(&data);
	}
	ft_exit(&data, data.out);
}
