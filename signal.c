/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-sant <fde-sant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 10:57:05 by alborghi          #+#    #+#             */
/*   Updated: 2025/03/22 16:22:32 by fde-sant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_signal;

void	sig_quit(int signum)
{
	(void)signum;
	ft_printe("Quit (core dumped)\n");
	exit(131);
}

void	sig_here(int signum)
{
	g_signal = signum;
	printf("\n");
	close(0);
}

void	sig_ignore(int signum)
{
	g_signal = signum;
}

void	init_signals(void)
{
	signal(SIGINT, new_prompt);
	signal(SIGQUIT, sig_ignore);
}

void	new_prompt(int signum)
{
	g_signal = signum;
	ft_printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}
