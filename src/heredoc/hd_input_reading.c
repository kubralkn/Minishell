/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_input_reading.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:26:57 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/22 09:57:27 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "heredoc.h"
#include "libft.h"
#include "signal_handler.h"
#include "utils.h"

static char	*append_heredoc_line(t_context *ctx, char *heredoc_lines,
		const char *line)
{
	char	*temp;
	char	*line_with_newline;

	line_with_newline = gc_strjoin(&ctx->gc, line, "\n");
	if (ft_strlen(heredoc_lines) > 0)
	{
		temp = gc_strjoin(&ctx->gc, heredoc_lines, line_with_newline);
		gc_free(&ctx->gc, heredoc_lines);
		heredoc_lines = temp;
	}
	else
		heredoc_lines = gc_strdup(&ctx->gc, line_with_newline);
	gc_free(&ctx->gc, line_with_newline);
	return (heredoc_lines);
}

static int	handle_heredoc_signals(t_context *ctx, char **heredoc_lines)
{
	if (g_signal == SIGINT)
	{
		if (ctx->shell)
			ctx->shell->last_exit_status = 130;
		gc_free(&ctx->gc, *heredoc_lines);
		*heredoc_lines = NULL;
		return (-1);
	}
	return (0);
}

static int	handle_heredoc_eof(const char *delimiter, int line_number)
{
	print_warning_message_with_line(delimiter, line_number);
	return (0);
}

static int	process_heredoc_input(t_context *ctx, const char *delimiter,
		char **heredoc_lines, int *line_number)
{
	char	*line;

	line = gc_readline(&ctx->gc, "> ");
	(*line_number)++;
	if (!line || g_signal == SIGINT)
	{
		if (line)
			gc_free(&ctx->gc, line);
		if (g_signal == SIGINT)
			return (handle_heredoc_signals(ctx, heredoc_lines));
		if (!line)
			return (handle_heredoc_eof(delimiter, *line_number));
		return (0);
	}
	if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
		&& ft_strlen(line) == ft_strlen(delimiter))
		return (gc_free(&ctx->gc, line), 0);
	*heredoc_lines = append_heredoc_line(ctx, *heredoc_lines, line);
	return (gc_free(&ctx->gc, line), 1);
}

char	*read_heredoc_content(t_context *ctx, const char *delimiter)
{
	char	*heredoc_lines;
	int		result;
	int		line_number;

	line_number = 0;
	set_signal_handler(HERE_SIG);
	heredoc_lines = gc_strdup(&ctx->gc, "");
	while (1)
	{
		result = process_heredoc_input(ctx, delimiter, &heredoc_lines,
				&line_number);
		if (result == -1)
		{
			set_signal_handler(PROMT_SIG);
			return (NULL);
		}
		if (result == 0)
			break ;
	}
	set_signal_handler(PROMT_SIG);
	return (heredoc_lines);
}
