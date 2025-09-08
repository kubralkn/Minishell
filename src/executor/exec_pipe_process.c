/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_process.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:58:55 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/25 19:42:32 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "exit_codes.h"
#include "signal_handler.h"
#include <sys/wait.h>

static int	wait_for_children(pid_t pid1, pid_t pid2)
{
	int	status1;
	int	status2;
	int	exit_code;

	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if ((status2 & 0x7F) != 0 && (status2 & 0x7F) == SIGQUIT)
		return (131);
	exit_code = handle_process_exit_code(status2);
	return (exit_code);
}

static int	create_pipe_processes(t_ast_node *ast, t_shell *shell,
		t_context *ctx, t_pipe_pids *pids)
{
	if (pipe(pids->pipefd) == -1)
		return (perror("pipe"), EXIT_GENERAL_ERROR);
	pids->pid1 = create_left_process(ast, shell, ctx, pids->pipefd);
	if (pids->pid1 == -1)
	{
		close(pids->pipefd[0]);
		close(pids->pipefd[1]);
		return (EXIT_GENERAL_ERROR);
	}
	pids->pid2 = create_right_process(ast, shell, ctx, pids->pipefd);
	if (pids->pid2 == -1)
	{
		close(pids->pipefd[0]);
		close(pids->pipefd[1]);
		kill(pids->pid1, SIGTERM);
		waitpid(pids->pid1, NULL, 0);
		return (EXIT_GENERAL_ERROR);
	}
	return (0);
}

static int	handle_pipe_processes(t_ast_node *ast, t_shell *shell,
		t_context *ctx, t_pipe_pids *pids)
{
	int	result;
	int	exit_code;

	set_signal_handler(EXEC_SIG);
	result = create_pipe_processes(ast, shell, ctx, pids);
	if (result != 0)
	{
		set_signal_handler(PROMT_SIG);
		return (result);
	}
	close(pids->pipefd[0]);
	close(pids->pipefd[1]);
	exit_code = wait_for_children(pids->pid1, pids->pid2);
	set_signal_handler(PROMT_SIG);
	return (exit_code);
}

int	execute_pipe(t_ast_node *ast, t_shell *shell, t_context *ctx)
{
	int			result;
	t_pipe_pids	pids;

	result = handle_pipe_processes(ast, shell, ctx, &pids);
	return (result);
}
