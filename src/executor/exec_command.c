/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:58:46 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/30 13:06:56 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "exit_codes.h"
#include "gc.h"
#include "libft.h"
#include "parser.h"
#include "utils.h"
#include "env.h"
#include <unistd.h>

void	init_exec_command(t_exec_command *exec_cmd, t_parser_command *cmd)
{
	exec_cmd->argv = cmd->args;
	exec_cmd->fd_in = 0;
	exec_cmd->fd_out = 1;
	exec_cmd->fd_err = 2;
}

void	cleanup_exec_command_fds(t_exec_command *exec_cmd)
{
	if (exec_cmd->fd_in != 0)
		close(exec_cmd->fd_in);
	if (exec_cmd->fd_out != 1)
		close(exec_cmd->fd_out);
	if (exec_cmd->fd_err != 2)
		close(exec_cmd->fd_err);
}

int	is_builtin_command(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	execute_command_logic(t_exec_command *exec_cmd, t_shell *shell,
		t_context *ctx)
{
	int	result;

	if (!exec_cmd->argv[0] || ft_strlen(exec_cmd->argv[0]) == 0)
	{
		set_exit_status(shell, EXIT_SUCCESS);
		return (EXIT_SUCCESS);
	}
	if (exec_cmd->argv[0] && ft_strcmp(exec_cmd->argv[0],
			"__EMPTY_QUOTES__") == 0)
	{
		print_error_message(NULL, "", "command not found");
		set_exit_status(shell, EXIT_COMMAND_NOT_FOUND);
		return (EXIT_COMMAND_NOT_FOUND);
	}
	if (is_builtin_command(exec_cmd->argv[0]))
		result = execute_builtin(exec_cmd, shell, ctx);
	else
		result = execute_external_command(exec_cmd, shell, ctx);
	return (result);
}

int	execute_and_cleanup(t_exec_command *exec_cmd, t_shell *shell,
		t_context *ctx, int saved_stdout)
{
	int	result;
	int	argc;

	result = execute_command_logic(exec_cmd, shell, ctx);
	argc = 0;
	while (exec_cmd->argv[argc])
		argc++;
	if (argc > 0 && exec_cmd->argv[argc - 1])
	{
		shell->last_arg = gc_strdup(&ctx->gc, exec_cmd->argv[argc - 1]);
		if (!shell->last_arg)
			shell->last_arg = NULL;
		else
			env_assign(ctx, "_", shell->last_arg);
	}
	dup2(saved_stdout, 1);
	close(saved_stdout);
	cleanup_exec_command_fds(exec_cmd);
	return (result);
}
