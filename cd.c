/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alborghi <alborghi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 10:52:08 by alborghi          #+#    #+#             */
/*   Updated: 2025/03/21 15:25:40 by alborghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_home(t_data *data, char *oldpwd)
{
	char	*tmp;

	if (get_env(data->env, "HOME") == NULL
		|| chdir(get_env(data->env, "HOME")) == -1)
	{
		if (get_env(data->env, "HOME") == NULL)
			ft_printe("minishell: cd: HOME not set\n");
		else
		{
			tmp = ft_strjoin("minishell: cd: ", get_env(data->env, "HOME"));
			perror(tmp);
			free(tmp);
		}
		free(oldpwd);
		return (1);
	}
	return (0);
}

int	ft_absolute_path(t_data *data, char *oldpwd)
{
	char	*tmp;

	if (get_env(data->env, "HOME") != NULL)
		tmp = ft_strjoin(get_env(data->env, "HOME"),
				data->cmds->args[0] + 1);
	else
		tmp = ft_strjoin(data->home, data->cmds->args[0] + 1);
	if (chdir(tmp) == -1)
	{
		tmp = ft_strjoin_free_2("minishell: cd: ", tmp);
		perror(tmp);
		free(tmp);
		free(oldpwd);
		return (1);
	}
	free(tmp);
	return (0);
}

int	ft_go_back(t_data *data, char *oldpwd)
{
	char	*tmp;

	if (get_env(data->env, "OLDPWD"))
	{
		if (chdir(get_env(data->env, "OLDPWD")) == -1)
		{
			tmp = ft_strjoin("minishell: cd: ",
					get_env(data->env, "OLDPWD"));
			perror(tmp);
			free(tmp);
			free(oldpwd);
			return (1);
		}
		ft_printf("%s\n", get_env(data->env, "OLDPWD"));
	}
	else
	{
		ft_printe("minishell: cd: OLDPWD not set\n");
		free(oldpwd);
		return (1);
	}
	return (0);
}

int	ft_handle_cases(t_data *data, char *oldpwd)
{
	char	*tmp;

	if (!data->cmds->args || data->cmds->args[0] == NULL
		|| data->cmds->args[0][0] == '\0')
	{
		if (ft_home(data, oldpwd) == 1)
			return (1);
	}
	else if (ft_strncmp(data->cmds->args[0], "~/", 2) == 0
		|| ft_strncmp(data->cmds->args[0], "~", 2) == 0)
	{
		if (ft_absolute_path(data, oldpwd) == 1)
			return (1);
	}
	else if (ft_strncmp(data->cmds->args[0], "-", 2) == 0)
	{
		if (ft_go_back(data, oldpwd) == 1)
			return (1);
	}
	else if (chdir(data->cmds->args[0]) == -1)
	{
		tmp = ft_strjoin("minishell: cd: ", data->cmds->args[0]);
		return (perror(tmp), free(tmp), free(oldpwd), 1);
	}
	return (0);
}

int	exec_cd(t_data *data)
{
	char	*oldpwd;

	if (!data->cmds)
		return (1);
	if (ft_char_mat_len(data->cmds->args) > 1)
		return (ft_printe("cd: too many arguments\n"), 1);
	oldpwd = getcwd(NULL, 0);
	if (ft_handle_cases(data, oldpwd) == 1)
		return (1);
	set_env(data->env, "OLDPWD", oldpwd);
	free(data->pwd);
	data->pwd = getcwd(NULL, 0);
	set_env(data->env, "PWD", getcwd(NULL, 0));
	return (0);
}
