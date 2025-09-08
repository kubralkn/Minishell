/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:19:40 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/30 12:10:08 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signal_handler.h"
#include "exit_codes.h"
#include <fcntl.h>

void	handle_sigint_here(int sig)
{
	g_signal = sig;
	rl_replace_line("", 0);
	rl_done = 1;
}

void	handle_sigint_exec(int sig)
{
	g_signal = sig;
}

void	handle_sigint_prompt(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigquit_exec(int sig)
{
	g_signal = sig;
}
