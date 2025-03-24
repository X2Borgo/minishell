/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alborghi <alborghi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 10:36:02 by alborghi          #+#    #+#             */
/*   Updated: 2025/03/24 08:58:33 by alborghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	if (ft_strlen_int(str) > 19 + (str[0] == '-' || str[0] == '+'))
		return (0);
	if (ft_strlen_int(str) < 19 + (str[0] == '-' || str[0] == '+'))
		return (1);
	if (str[0] == '-')
		return (ft_strncmp(str, "-9223372036854775808", 20) <= 0);
	if (str[0] == '+')
		return (ft_strncmp(str, "+9223372036854775807", 20) >= 0);
	return (ft_strncmp(str, "9223372036854775807", 19) >= 0);
}

int	convert_signal(char *str)
{
	unsigned long long	sig;
	int					sign;

	sig = 0;
	sign = 1;
	if (str[0] == '-' || str[0] == '+')
	{
		if (str[0] == '-')
			sign = -1;
		str++;
	}
	while (*str)
	{
		sig = sig * 10 + *str - '0';
		str++;
	}
	return (sig * sign % 256);
}

int	get_signal(int *sig, char **args, t_data *data)
{
	if (data->head->next == NULL)
		ft_printf("exit\n");
	if (args[0] == NULL)
		return (*sig = 0, 0);
	if (!is_numeric(args[0]))
	{
		*sig = 2;
		data->status = *sig;
		ft_printe("minishell: exit: %s: numeric argument required\n", args[0]);
		return (0);
	}
	*sig = convert_signal(args[0]);
	if (args[1] != NULL)
	{
		data->status = 1;
		ft_printe("minishell: exit: too many arguments\n");
		return (1);
	}
	data->status = *sig;
	return (0);
}

void	ft_exit_builtin(t_data *data)
{
	int	sig;

	sig = 0;
	if (get_signal(&sig, data->cmds->args, data) == 1)
		return ;
	ft_exit(data, sig);
}
