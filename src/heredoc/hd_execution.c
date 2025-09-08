/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_execution.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:26:55 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/27 09:57:50 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "exit_codes.h"
#include "expander.h"
#include "heredoc.h"
#include "libft.h"
#include "signal_handler.h"

static int	write_heredoc_content(t_context *ctx, const char *content, int fd)
{
	char	*expanded_line;

	expanded_line = expand_env_variables(content, ctx->shell, &ctx->gc);
	if (expanded_line)
	{
		write(fd, expanded_line, ft_strlen(expanded_line));
		gc_free(&ctx->gc, expanded_line);
	}
	else
		write(fd, content, ft_strlen(content));
	return (EXIT_SUCCESS);
}

static int	setup_heredoc_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (EXIT_GENERAL_ERROR);
	}
	return (EXIT_SUCCESS);
}

static char	*get_heredoc_content_by_index(t_context *ctx, int index)
{
	t_heredoc_content	*current;
	int					current_index;

	if (!ctx->heredoc_list || index < 0)
		return (NULL);
	current = ctx->heredoc_list;
	current_index = 0;
	while (current && current_index < index)
	{
		current = current->next;
		current_index++;
	}
	if (current)
		return (gc_strdup(&ctx->gc, current->content));
	return (NULL);
}

static int	process_heredoc_by_index(t_context *ctx, int fd, int index)
{
	char	*content;
	int		should_expand;

	content = get_heredoc_content_by_index(ctx, index);
	if (!content)
		return (0);
	should_expand = !ctx->heredoc_quoted;
	if (should_expand)
		write_heredoc_content(ctx, content, fd);
	else
		write(fd, content, ft_strlen(content));
	if (ft_strlen(content) > 0 && content[ft_strlen(content) - 1] != '\n')
		write(fd, "\n", 1);
	gc_free(&ctx->gc, content);
	return (1);
}

int	handle_heredoc_fd_redirect(t_context *ctx, t_exec_command *exec_cmd,
			t_shell *shell, int heredoc_index)
{
	int	pipefd[2];

	ctx->shell = shell;
	if (exec_cmd->fd_in != 0)
	{
		close(exec_cmd->fd_in);
		exec_cmd->fd_in = 0;
	}
	if (setup_heredoc_pipe(pipefd) != EXIT_SUCCESS)
		return (EXIT_GENERAL_ERROR);
	if (!process_heredoc_by_index(ctx, pipefd[1], heredoc_index))
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (EXIT_GENERAL_ERROR);
	}
	close(pipefd[1]);
	exec_cmd->fd_in = pipefd[0];
	return (EXIT_SUCCESS);
}
