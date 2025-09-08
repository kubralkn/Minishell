/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external_process.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:58:48 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/25 19:43:50 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "executor.h"
#include "exit_codes.h"
#include "signal_handler.h"
#include "utils.h"
#include <sys/stat.h>
#include <sys/wait.h>

static int	setup_child_redirections(t_exec_command *exec_cmd)
{
	if (exec_cmd->fd_in != 0)
	{
		if (dup2(exec_cmd->fd_in, 0) == -1)
		{
			perror("dup2");
			return (EXIT_GENERAL_ERROR);
		}
	}
	if (exec_cmd->fd_out != 1)
	{
		if (dup2(exec_cmd->fd_out, 1) == -1)
		{
			perror("dup2");
			return (EXIT_GENERAL_ERROR);
		}
	}
	if (exec_cmd->fd_err != 2)
	{
		if (dup2(exec_cmd->fd_err, 2) == -1)
		{
			perror("dup2");
			return (EXIT_GENERAL_ERROR);
		}
	}
	return (EXIT_SUCCESS);
}

static void	execute_child_process(t_exec_command *exec_cmd, char *cmd_path,
		char **env_array, t_context *ctx)
{
	struct stat	st;

	restore_signal_handlers();
	if (setup_child_redirections(exec_cmd) == EXIT_GENERAL_ERROR)
	{
		clean_exit(ctx);
		exit(EXIT_GENERAL_ERROR);
	}
	if (exec_cmd->fd_in != 0)
		close(exec_cmd->fd_in);
	if (exec_cmd->fd_out != 1)
		close(exec_cmd->fd_out);
	if (exec_cmd->fd_err != 2)
		close(exec_cmd->fd_err);
	if (stat(cmd_path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		clean_exit(ctx);
		exit(EXIT_CANNOT_EXECUTE);
	}
	execve(cmd_path, exec_cmd->argv, env_array);
	clean_exit(ctx);
	exit(EXIT_GENERAL_ERROR);
}

static int	handle_process_status(int status, t_shell *shell)
{
	int	exit_status;

	exit_status = handle_process_exit_code(status);
	set_exit_status(shell, exit_status);
	return (exit_status);
}

static pid_t	setup_and_fork_external_command(t_exec_command *exec_cmd,
		t_shell *shell, t_context *ctx)
{
	pid_t	pid;
	char	*cmd_path;
	char	**env_array;

	cmd_path = get_executable_path(exec_cmd->argv[0], ctx, shell);
	if (!cmd_path)
		return (-2);
	env_array = env_to_array(ctx);
	if (!env_array)
		return (gc_free(&ctx->gc, cmd_path), -3);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		gc_free(&ctx->gc, cmd_path);
		gc_free_string_array(&ctx->gc, env_array);
		return (-1);
	}
	if (pid == 0)
		execute_child_process(exec_cmd, cmd_path, env_array, ctx);
	gc_free(&ctx->gc, cmd_path);
	gc_free_string_array(&ctx->gc, env_array);
	return (pid);
}

int	execute_external_command(t_exec_command *exec_cmd, t_shell *shell,
		t_context *ctx)
{
	pid_t	pid;
	int		status;

	set_signal_handler(EXEC_SIG);
	pid = setup_and_fork_external_command(exec_cmd, shell, ctx);
	if (pid == -2)
	{
		set_signal_handler(PROMT_SIG);
		return (shell->last_exit_status);
	}
	if (pid == -3)
		return (set_signal_handler(PROMT_SIG), EXIT_GENERAL_ERROR);
	if (pid == -1)
		return (set_signal_handler(PROMT_SIG), EXIT_GENERAL_ERROR);
	waitpid(pid, &status, 0);
	set_signal_handler(PROMT_SIG);
	if ((status & 0x7F) != 0 && (status & 0x7F) == SIGQUIT)
	{
		if (!ctx->in_pipe)
			write(2, "Quit (core dumped)\n", 19);
		return (131);
	}
	return (handle_process_status(status, shell));
}
