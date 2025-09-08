/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:59:00 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/25 19:44:00 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "env.h"
#include "exit_codes.h"
#include "utils.h"
#include <fcntl.h>
#include <unistd.h>
#include <heredoc.h>

static int	handle_input_redirect(t_redirect *redirect,
		t_exec_command *exec_cmd)
{
	int	fd;

	if (access(redirect->filename, F_OK) != 0)
	{
		print_error_message(NULL, redirect->filename,
			"No such file or directory");
		return (EXIT_GENERAL_ERROR);
	}
	if (access(redirect->filename, R_OK) != 0)
	{
		print_error_message(NULL, redirect->filename, "Permission denied");
		return (EXIT_GENERAL_ERROR);
	}
	fd = open(redirect->filename, O_RDONLY);
	if (fd == -1)
	{
		print_error_message(NULL, redirect->filename,
			"No such file or directory");
		return (EXIT_GENERAL_ERROR);
	}
	if (exec_cmd->fd_in != 0)
		close(exec_cmd->fd_in);
	exec_cmd->fd_in = fd;
	return (EXIT_SUCCESS);
}

static int	check_parent_dir_write_permission(t_context *ctx, char *filename)
{
	char	*parent_dir;
	char	*last_slash;
	int		result;

	last_slash = ft_strrchr(filename, '/');
	if (!last_slash)
		return (access(".", W_OK) == 0);
	if (last_slash == filename)
		parent_dir = gc_strdup(&ctx->gc, "/");
	else
		parent_dir = gc_substr(&ctx->gc, filename, 0, last_slash - filename);
	if (!parent_dir)
		return (0);
	result = access(parent_dir, W_OK);
	return (result == 0);
}

static int	handle_output_redirect(t_context *ctx, t_redirect *redirect,
		t_exec_command *exec_cmd)
{
	int	fd;

	fd = open(redirect->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		if (access(redirect->filename, F_OK) == 0)
		{
			if (is_directory(redirect->filename))
				print_error_message(NULL, redirect->filename, "Is a directory");
			else
				print_error_message(NULL, redirect->filename,
					"Permission denied");
		}
		else if (!check_parent_dir_write_permission(ctx, redirect->filename))
			print_error_message(NULL, redirect->filename, "Permission denied");
		else
			print_error_message(NULL, redirect->filename,
				"No such file or directory");
		return (EXIT_GENERAL_ERROR);
	}
	if (exec_cmd->fd_out != 1)
		close(exec_cmd->fd_out);
	exec_cmd->fd_out = fd;
	return (EXIT_SUCCESS);
}

static int	handle_append_redirect(t_context *ctx, t_redirect *redirect,
		t_exec_command *exec_cmd)
{
	int	fd;

	fd = open(redirect->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		if (access(redirect->filename, F_OK) == 0)
		{
			if (is_directory(redirect->filename))
				print_error_message(NULL, redirect->filename, "Is a directory");
			else
				print_error_message(NULL, redirect->filename,
					"Permission denied");
		}
		else if (!check_parent_dir_write_permission(ctx, redirect->filename))
			print_error_message(NULL, redirect->filename, "Permission denied");
		else
			print_error_message(NULL, redirect->filename,
				"No such file or directory");
		return (EXIT_GENERAL_ERROR);
	}
	if (exec_cmd->fd_out != 1)
		close(exec_cmd->fd_out);
	exec_cmd->fd_out = fd;
	return (EXIT_SUCCESS);
}

int	handle_single_redirect(t_context *ctx, t_redirect *redirect,
		t_exec_command *exec_cmd, t_shell *shell)
{
	if (redirect->type == REDIR_IN)
		return (handle_input_redirect(redirect, exec_cmd));
	else if (redirect->type == REDIR_OUT)
		return (handle_output_redirect(ctx, redirect, exec_cmd));
	else if (redirect->type == REDIR_APPEND)
		return (handle_append_redirect(ctx, redirect, exec_cmd));
	else if (redirect->type == REDIR_HEREDOC)
	{
		if (redirect->heredoc_index >= 0)
			return (handle_heredoc_fd_redirect(ctx, exec_cmd, shell,
					redirect->heredoc_index));
	}
	return (EXIT_GENERAL_ERROR);
}
