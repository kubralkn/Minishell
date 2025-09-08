/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:58:44 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/30 13:06:56 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "env.h"
#include "executor.h"
#include "exit_codes.h"
#include "utils.h"
#include <stdio.h>
#include <unistd.h>

static int	dispatch_builtin_command(t_exec_command *exec_cmd, t_shell *shell,
		t_context *ctx)
{
	char	*cmd;
	int		result;

	cmd = exec_cmd->argv[0];
	if (ft_strcmp(cmd, "echo") == 0)
		return (built_echo(exec_cmd));
	else if (ft_strcmp(cmd, "cd") == 0)
		return (built_cd(ctx, exec_cmd));
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (built_pwd(ctx));
	else if (ft_strcmp(cmd, "export") == 0)
	{
		result = built_export(ctx, exec_cmd->argv);
		return (shell->env_list = ctx->env, result);
	}
	else if (ft_strcmp(cmd, "unset") == 0)
	{
		result = built_unset(ctx, exec_cmd->argv);
		return (shell->env_list = ctx->env, result);
	}
	else if (ft_strcmp(cmd, "env") == 0)
		return (built_env(ctx, exec_cmd));
	else if (ft_strcmp(cmd, "exit") == 0)
		return (built_exit(exec_cmd, shell, ctx));
	return (EXIT_GENERAL_ERROR);
}

static void	restore_builtin_fds(int saved_fd_in, int saved_fd_out)
{
	if (saved_fd_in != -1)
	{
		dup2(saved_fd_in, 0);
		close(saved_fd_in);
	}
	if (saved_fd_out != -1)
	{
		dup2(saved_fd_out, 1);
		close(saved_fd_out);
	}
}

static int	setup_builtin_output(t_exec_command *exec_cmd)
{
	int	saved_fd_out;

	saved_fd_out = -1;
	if (exec_cmd->fd_out != 1)
	{
		saved_fd_out = dup(1);
		if (dup2(exec_cmd->fd_out, 1) == -1)
		{
			perror("dup2");
			if (saved_fd_out != -1)
				close(saved_fd_out);
			return (-1);
		}
	}
	return (saved_fd_out);
}

static int	setup_builtin_input(t_exec_command *exec_cmd)
{
	int	saved_fd_in;

	saved_fd_in = -1;
	if (exec_cmd->fd_in != 0)
	{
		saved_fd_in = dup(0);
		if (dup2(exec_cmd->fd_in, 0) == -1)
		{
			perror("dup2");
			if (saved_fd_in != -1)
				close(saved_fd_in);
			return (-1);
		}
	}
	return (saved_fd_in);
}

int	execute_builtin(t_exec_command *exec_cmd, t_shell *shell, t_context *ctx)
{
	int	result;
	int	saved_fd_in;
	int	saved_fd_out;

	saved_fd_in = setup_builtin_input(exec_cmd);
	if (saved_fd_in == -1 && exec_cmd->fd_in != 0)
		return (EXIT_GENERAL_ERROR);
	saved_fd_out = setup_builtin_output(exec_cmd);
	if (saved_fd_out == -1 && exec_cmd->fd_out != 1)
	{
		restore_builtin_fds(saved_fd_in, -1);
		return (EXIT_GENERAL_ERROR);
	}
	result = dispatch_builtin_command(exec_cmd, shell, ctx);
	restore_builtin_fds(saved_fd_in, saved_fd_out);
	set_exit_status(shell, result);
	return (result);
}
