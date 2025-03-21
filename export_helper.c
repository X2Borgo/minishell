/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alborghi <alborghi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:46:03 by alborghi          #+#    #+#             */
/*   Updated: 2025/03/21 15:26:16 by alborghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	substitute_env_var(char **var, char *arg)
{
	if (ft_strchr(arg, '=') == NULL)
		return (0);
	if (ft_strchr(arg, '+') != NULL)
	{
		if (ft_strchr(*var, '=') == NULL)
			*var = ft_strjoin_free_1(*var, "=");
		*var = ft_strjoin_free_1(*var, ft_strchr(arg, '=') + 1);
		if (!*var)
			return (1);
		return (0);
	}
	free(*var);
	*var = ft_strdup(arg);
	if (!*var)
		return (1);
	return (0);
}

int	append_env_var(t_env *last, char *arg, int is_env)
{
	last->next = (t_env *)malloc(sizeof(t_env));
	if (!last->next)
		return (1);
	if (ft_strchr(arg, '+') != NULL)
	{
		last->next->var = ft_strndup(arg, ft_strchr(arg, '+') - arg);
		if (!last->next->var)
			return (1);
		last->next->var = ft_strjoin_free_1(last->next->var,
				ft_strchr(arg, '='));
		if (!last->next->var)
			return (1);
	}
	else
		last->next->var = ft_strdup(arg);
	if (!last->next->var)
		return (1);
	last->next->is_env = is_env;
	last->next->next = NULL;
	return (0);
}
