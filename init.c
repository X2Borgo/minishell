/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alborghi <alborghi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:29:44 by alborghi          #+#    #+#             */
/*   Updated: 2025/03/21 16:28:05 by alborghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_env_cycle(t_env **tmp, char **env, int i)
{
	(*tmp)->var = ft_strdup(env[i]);
	if (!(*tmp)->var)
		return (1);
	(*tmp)->is_env = TRUE;
	if (env[i + 1])
	{
		(*tmp)->next = (t_env *)ft_calloc(1, sizeof(t_env));
		if (!(*tmp)->next)
			return (1);
		(*tmp) = (*tmp)->next;
	}
	return (0);
}

t_env	*init_env(char **env)
{
	t_env	*my_env;
	t_env	*tmp;
	int		i;

	if (!env || !*env)
		return (NULL);
	my_env = (t_env *)malloc(sizeof(t_env));
	if (!my_env)
		return (NULL);
	i = 0;
	tmp = my_env;
	while (env[i])
	{
		if (init_env_cycle(&tmp, env, i) == 1)
			return (free_env(my_env), NULL);
		i++;
	}
	tmp->next = NULL;
	return (my_env);
}

int	init_data(t_data *data, char **env)
{
	data->env = init_env(env);
	if (!data->env)
		return (1);
	data->cmds = NULL;
	data->head = NULL;
	data->status = 0;
	data->fds = NULL;
	data->pids = NULL;
	data->out = 0;
	data->home = ft_strdup(get_env(data->env, "HOME"));
	if (!data->home)
		return (1);
	data->pwd = ft_strdup(get_env(data->env, "PWD"));
	if (!data->pwd)
		return (1);
	data->stdi = dup(0);
	data->stdo = dup(1);
	data->prompt = dup(1);
	data->history = ft_strjoin(data->home, "/.mini_history");
	if (open(data->history, O_CREAT | O_RDWR, 0666) == -1)
		return (perror("minishell: ~/.mini_history"), 1);
	return (0);
}
