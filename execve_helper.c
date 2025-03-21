/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alborghi <alborghi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:56:38 by alborghi          #+#    #+#             */
/*   Updated: 2025/03/21 15:58:45 by alborghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_path(char *cmd, char *path)
{
	char	**paths;
	int		i;
	char	*tmp;

	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
			return (ft_free_mat_char(paths), NULL);
		tmp = ft_strjoin_free_1(tmp, cmd);
		if (!tmp)
			return (ft_free_mat_char(paths), NULL);
		if (access(tmp, F_OK | X_OK) == 0)
			return (ft_free_mat_char(paths), tmp);
		free(tmp);
		i++;
	}
	ft_free_mat_char(paths);
	return (NULL);
}

char	**get_args(t_cmd *cmds)
{
	char	**argv;
	int		i;

	argv = (char **)ft_calloc(ft_char_mat_len(cmds->args) + 2, sizeof(char *));
	if (!argv)
		return (NULL);
	argv[0] = ft_strdup(cmds->cmd);
	i = 0;
	while (cmds->args && cmds->args[i])
	{
		argv[i + 1] = ft_strdup(cmds->args[i]);
		i++;
	}
	argv[i + 1] = NULL;
	return (argv);
}

char	**env_to_mat(t_env *env)
{
	char	**mat;
	int		i;
	t_env	*tmp;

	i = 0;
	tmp = env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	mat = (char **)ft_calloc(i + 1, sizeof(char *));
	if (!mat)
		return (NULL);
	i = 0;
	tmp = env;
	while (tmp)
	{
		mat[i] = ft_strdup(tmp->var);
		i++;
		tmp = tmp->next;
	}
	mat[i] = NULL;
	return (mat);
}
