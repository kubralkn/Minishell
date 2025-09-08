/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:58:57 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/27 09:56:29 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "executor.h"
#include "exit_codes.h"
#include "libft.h"
#include "parser.h"
#include "signal_handler.h"
#include "utils.h"

static void	setup_left_child_process(t_ast_node *ast, t_shell *shell,
		t_context *ctx, int write_fd)
{
	int	exit_code;

	restore_signal_handlers();
	if (dup2(write_fd, 1) == -1)
	{
		perror("dup2");
		exit(EXIT_GENERAL_ERROR);
	}
	close(write_fd);
	ctx->in_pipe = 1;
	exit_code = execute_ast(ast->left, shell, ctx);
	clean_exit(ctx);
	exit(exit_code);
}

pid_t	create_left_process(t_ast_node *ast, t_shell *shell, t_context *ctx,
		int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		setup_left_child_process(ast, shell, ctx,
			pipefd[1]);
	}
	return (pid);
}

static void	setup_right_child_process(t_ast_node *ast, t_shell *shell,
		t_context *ctx, int read_fd)
{
	int	exit_code;

	restore_signal_handlers();
	dup2(read_fd, 0);
	close(read_fd);
	ctx->in_pipe = 1;
	exit_code = execute_ast(ast->right, shell, ctx);
	clean_exit(ctx);
	exit(exit_code);
}

pid_t	create_right_process(t_ast_node *ast, t_shell *shell, t_context *ctx,
		int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		close(pipefd[1]);
		setup_right_child_process(ast, shell, ctx,
			pipefd[0]);
	}
	return (pid);
}
