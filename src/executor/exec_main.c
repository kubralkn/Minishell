/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 19:42:50 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/25 19:42:53 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "executor.h"
#include "exit_codes.h"
#include "expander.h"
#include "libft.h"
#include "parser.h"
#include "signal_handler.h"
#include "utils.h"

static int	handle_redirections(t_context *ctx, t_redirect *redirections,
		t_exec_command *exec_cmd, t_shell *shell)
{
	t_redirect	*current;

	current = redirections;
	while (current)
	{
		if (handle_single_redirect(ctx, current, exec_cmd,
				shell) != EXIT_SUCCESS)
			return (EXIT_GENERAL_ERROR);
		current = current->next;
	}
	return (EXIT_SUCCESS);
}

int	execute_command(t_parser_command *cmd, t_shell *shell, t_context *ctx)
{
	t_exec_command	exec_cmd;
	int				saved_stdout;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (EXIT_SUCCESS);
	if (expand_command_args(cmd, shell, &ctx->gc) == -1)
		return (EXIT_GENERAL_ERROR);
	if (cmd->args[0] && ft_strlen(cmd->args[0]) == 0)
	{
		print_error_message(NULL, "", "command not found");
		return (EXIT_COMMAND_NOT_FOUND);
	}
	if (expand_redirects(cmd, ctx) == -1)
		return (EXIT_GENERAL_ERROR);
	init_exec_command(&exec_cmd, cmd);
	saved_stdout = dup(1);
	if (saved_stdout == -1)
		return (EXIT_GENERAL_ERROR);
	if (handle_redirections(ctx, cmd->redirects, &exec_cmd,
			shell) != EXIT_SUCCESS)
	{
		close(saved_stdout);
		return (cleanup_exec_command_fds(&exec_cmd), EXIT_GENERAL_ERROR);
	}
	return (execute_and_cleanup(&exec_cmd, shell, ctx, saved_stdout));
}

int	execute_ast(t_ast_node *ast, t_shell *shell, t_context *ctx)
{
	if (!ast)
		return (EXIT_SUCCESS);
	if (ast->type == AST_COMMAND)
		return (execute_command(ast->command, shell, ctx));
	else if (ast->type == AST_PIPE)
		return (execute_pipe(ast, shell, ctx));
	return (EXIT_GENERAL_ERROR);
}
