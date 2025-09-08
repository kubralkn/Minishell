/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:58:27 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/14 15:58:27 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "env.h"
#include "executor.h"
#include "exit_codes.h"
#include "gc.h"
#include "utils.h"
#include <stdio.h>

static int	process_double_arg(t_exec_command *command, t_shell *shell,
		t_context *ctx)
{
	int	exit_code;
	int	normalized;

	if (check_exit_arg(command->argv[1], &exit_code, &ctx->gc))
	{
		if (shell)
		{
			normalized = normalize_exit_code(exit_code);
			shell->last_exit_status = normalized;
			shell->should_exit = 1;
			return (normalized);
		}
		return (normalize_exit_code(exit_code));
	}
	else
	{
		print_error_message("exit", command->argv[1],
			"numeric argument required");
		if (shell)
		{
			shell->last_exit_status = 2;
			shell->should_exit = 1;
		}
		return (2);
	}
}

static int	handle_too_many_args(t_exec_command *command, t_shell *shell,
		t_context *ctx)
{
	int	exit_code;

	if (!check_exit_arg(command->argv[1], &exit_code, &ctx->gc))
	{
		print_error_message("exit", command->argv[1],
			"numeric argument required");
		if (shell)
		{
			shell->last_exit_status = 2;
			shell->should_exit = 1;
		}
		return (2);
	}
	else
	{
		print_error_message("exit", NULL, "too many arguments");
		if (shell)
			shell->last_exit_status = EXIT_GENERAL_ERROR;
		return (EXIT_GENERAL_ERROR);
	}
}

static int	handle_exit_arg(t_exec_command *command, t_shell *shell,
		t_context *ctx)
{
	int	arg_count;

	arg_count = argument_count(command->argv);
	if (arg_count == 1)
	{
		shell->should_exit = 1;
		if (shell)
			return (shell->last_exit_status);
		return (EXIT_SUCCESS);
	}
	else if (arg_count == 2)
		return (process_double_arg(command, shell, ctx));
	else
		return (handle_too_many_args(command, shell, ctx));
}

int	built_exit(t_exec_command *command, t_shell *shell, t_context *ctx)
{
	printf("exit\n");
	if (!command || !command->argv)
	{
		if (shell)
		{
			shell->should_exit = 1;
		}
		return (EXIT_SUCCESS);
	}
	return (handle_exit_arg(command, shell, ctx));
}
