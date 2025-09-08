/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:56:49 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/18 16:56:49 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_HANDLER_H
# define SIGNAL_HANDLER_H

# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_context	t_context;

extern int					g_signal;

typedef enum e_signal
{
	HERE_SIG,
	EXEC_SIG,
	PROMT_SIG
}	t_signal;

void	set_signal_handler(t_signal s_type);
void	handle_sigint_here(int sig);
void	handle_sigint_exec(int sig);
void	handle_sigint_prompt(int sig);
void	handle_sigquit_exec(int sig);
void	restore_signal_handlers(void);
int		do_noop(void);

#endif
