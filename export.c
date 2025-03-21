/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alborghi <alborghi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 14:01:27 by alborghi          #+#    #+#             */
/*   Updated: 2025/03/21 16:09:56 by alborghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_put_export(t_env *env)
{
	int		i;
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		i = 0;
		while (tmp->var[i] && tmp->var[i] != '=')
			i++;
		if (tmp->var[i] == '=')
			i++;
		write(1, "declare -x ", 11);
		write(1, tmp->var, i);
		if (tmp->var[i])
			ft_printf("\"%s\"", &tmp->var[i]);
		else if (tmp->var[i - 1] == '=')
			ft_printf("\"\"");
		ft_printf("\n");
		tmp = tmp->next;
	}
}

//TODO: add free were needed
//TODO: add arg error handling
//TODO: add export += arg case
int	export_cmd(char *arg, t_env *env)
{
	t_env	*tmp;
	int		is_env;

	is_env = TRUE;
	if (ft_strchr(arg, '=') == NULL)
		is_env = FALSE;
	tmp = env;
	while (tmp)
	{
		if (check_key(tmp->var, arg) == TRUE)
		{
			if (substitute_env_var(&(tmp->var), arg) == 1)
				return (1);
			tmp->is_env = is_env;
			return (0);
		}
		if (!tmp->next)
			return (append_env_var(tmp, arg, is_env));
		tmp = tmp->next;
	}
	ft_put_export(env);
	return (0);
}

int	check_arg(char *arg, char *cmd)
{
	int	i;

	i = 0;
	if (ft_isdigit(arg[0]) == 1)
		return (ft_printe("minishell: %s: `%s': not a valid identifier\n",
				cmd, arg), 1);
	while (arg[i] && arg[i] != '=' && arg[i] != '+')
	{
		if (ft_isalnum(arg[i]) == 0 && arg[i] != '_')
		{
			ft_printe("minishell: %s: `%s': not a valid identifier\n", cmd,
				arg);
			return (1);
		}
		i++;
	}
	if (i == 0)
		return (ft_printe("minishell: %s: `%s': not a valid identifier\n",
				cmd, arg), 1);
	if (arg[i] == '+' && arg[i + 1] != '=')
		return (ft_printe("minishell: %s: `%s': not a valid identifier\n",
				cmd, arg), 1);
	return (0);
}

int	exec_export(t_cmd *cmds, t_env *env)
{
	int		i;

	if (!cmds->args || !cmds->args[0])
		return (ft_put_export(env), 0);
	i = 0;
	while (cmds->args[i])
	{
		if (check_arg(cmds->args[i], "export") == 1)
		{
			i++;
			continue ;
		}
		else if (export_cmd(cmds->args[i], env) == 1)
			return (1);
		i++;
	}
	return (0);
}
