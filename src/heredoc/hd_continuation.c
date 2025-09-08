/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_continuation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:26:46 by kalkan            #+#    #+#             */
/*   Updated: 2025/09/01 11:00:55 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "heredoc.h"
#include "libft.h"

static t_heredoc_content	*reverse_heredoc_list(t_heredoc_content *head)
{
	t_heredoc_content	*prev;
	t_heredoc_content	*current;
	t_heredoc_content	*next;

	prev = NULL;
	current = head;
	while (current)
	{
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}
	return (prev);
}

static int	create_heredoc_content(t_context *ctx, char *heredoc_lines)
{
	t_heredoc_content	*new_heredoc;

	new_heredoc = (t_heredoc_content *)gc_alloc(&ctx->gc,
			sizeof(t_heredoc_content));
	if (!new_heredoc)
	{
		gc_free(&ctx->gc, heredoc_lines);
		return (0);
	}
	new_heredoc->content = heredoc_lines;
	new_heredoc->next = ctx->heredoc_list;
	ctx->heredoc_list = new_heredoc;
	if (ctx->current_heredoc_content)
		gc_free(&ctx->gc, ctx->current_heredoc_content);
	ctx->current_heredoc_content = gc_strdup(&ctx->gc, heredoc_lines);
	return (1);
}

static char	*process_heredoc_delimiters(t_context *ctx, char **delimiters)
{
	char					*heredoc_lines;
	char					*final_delimiter;
	int						i;
	int						was_quoted;

	i = 0;
	while (delimiters[i])
	{
		if (ft_strchr(delimiters[i], '"') != NULL
			|| ft_strchr(delimiters[i], '\'') != NULL)
			was_quoted = 1;
		else
			was_quoted = 0;
		final_delimiter = process_delimiter_quotes(ctx, delimiters[i]);
		ctx->heredoc_quoted = was_quoted;
		heredoc_lines = read_heredoc_content(ctx, final_delimiter);
		if (!heredoc_lines)
			return (gc_free(&ctx->gc, final_delimiter), NULL);
		if (!create_heredoc_content(ctx, heredoc_lines))
			return (gc_free(&ctx->gc, final_delimiter), NULL);
		gc_free(&ctx->gc, final_delimiter);
		gc_free(&ctx->gc, delimiters[i]);
		i++;
	}
	return (ctx->current_heredoc_content);
}

char	*handle_heredoc(t_context *ctx, const char *initial_line,
	t_token *token_list)
{
	char	**delimiters;
	char	*full_command;
	char	*result;
	int		i;

	delimiters = get_all_heredoc_delimiters(ctx, token_list);
	if (!delimiters)
		return (gc_strdup(&ctx->gc, initial_line));
	full_command = gc_strdup(&ctx->gc, initial_line);
	result = process_heredoc_delimiters(ctx, delimiters);
	if (!result)
	{
		gc_free(&ctx->gc, full_command);
		i = 0;
		while (delimiters[i])
			gc_free(&ctx->gc, delimiters[i++]);
		return (gc_free(&ctx->gc, delimiters), NULL);
	}
	ctx->heredoc_list = reverse_heredoc_list(ctx->heredoc_list);
	gc_free(&ctx->gc, delimiters);
	return (full_command);
}
