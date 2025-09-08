/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:19:40 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/14 15:19:40 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signal_handler.h"
#include "exit_codes.h"

int	g_signal = 0;

void	restore_signal_handlers(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

int	do_noop(void)
{
	return (0);
}

void	set_signal_handler(t_signal s_type)
{
	g_signal = 0;
	if (s_type == HERE_SIG)
	{
		rl_event_hook = do_noop;
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, handle_sigint_here);
	}
	else if (s_type == EXEC_SIG)
	{
		rl_event_hook = NULL;
		signal(SIGQUIT, handle_sigquit_exec);
		signal(SIGINT, handle_sigint_exec);
	}
	else if (s_type == PROMT_SIG)
	{
		rl_event_hook = NULL;
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, handle_sigint_prompt);
	}
}
