/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alborghi <alborghi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:58:50 by alborghi          #+#    #+#             */
/*   Updated: 2025/03/21 14:59:12 by alborghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_key(char *var, char *key)
{
	int	i;

	if (!var || !key)
		return (FALSE);
	i = 0;
	while (var[i] && key[i] && var[i] == key[i] && var[i] != '='
		&& key[i] != '+')
		i++;
	if ((var[i] == '=' || var[i] == '\0' || var[i] == '+')
		&& (key[i] == '\0' || key[i] == '=' || key[i] == '+'))
		return (TRUE);
	return (FALSE);
}

char	*get_env(t_env *env, char *key)
{
	t_env	*tmp;

	if (!env || !key)
		return (NULL);
	tmp = env;
	while (tmp)
	{
		if (check_key(tmp->var, key) == TRUE && tmp->is_env == TRUE)
			return (ft_strchr(tmp->var, '=') + 1);
		tmp = tmp->next;
	}
	return (NULL);
}

int	set_env(t_env *env, char *key, char *value)
{
	t_env	*tmp;
	t_env	*last;
	char	*tmp_var;

	if (!env || !key || !value)
		return (1);
	tmp = env;
	last = env;
	while (tmp)
	{
		if (check_key(tmp->var, key) == TRUE)
		{
			free(tmp->var);
			tmp->var = ft_strjoin_free_1_2(ft_strjoin(key, "="), value);
			return (0);
		}
		last = tmp;
		tmp = tmp->next;
	}
	tmp_var = ft_strjoin_free_1_2(ft_strjoin(key, "="),
			value);
	if (!tmp_var)
		return (1);
	append_env_var(last, tmp_var, TRUE);
	return (free(tmp_var), 1);
}
