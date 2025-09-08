/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:29:50 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/14 14:29:50 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static int	handle_special_tokens(t_lexer_ctx *ctx, int is_adjacent)
{
	if (handle_pipe(ctx))
	{
		*(ctx->last_end) = *(ctx->i);
		return (1);
	}
	if (handle_redirect(ctx))
	{
		*(ctx->last_end) = *(ctx->i);
		return (1);
	}
	if (handle_dollar_quotes(ctx, is_adjacent))
	{
		*(ctx->last_end) = *(ctx->i);
		return (1);
	}
	if (handle_quotes(ctx, is_adjacent))
	{
		*(ctx->last_end) = *(ctx->i);
		return (1);
	}
	return (0);
}

static void	handle_word_token(t_lexer_ctx *ctx, int is_adjacent)
{
	char	*word;

	word = get_word(ctx->line, ctx->i, ctx->gc);
	add_token_ctx(ctx, TOKEN_WORD, word, is_adjacent);
	(*(ctx->i))++;
	*(ctx->last_end) = *(ctx->i);
}

static void	lexer_next_token(t_lexer_ctx *ctx)
{
	int	is_adjacent;
	int	start_pos;

	start_pos = *(ctx->i);
	if (*(ctx->last_end) == start_pos)
		is_adjacent = 1;
	else
		is_adjacent = 0;
	if (handle_special_tokens(ctx, is_adjacent))
		return ;
	handle_word_token(ctx, is_adjacent);
}

void	lexer(const char *line, t_token **token_list, t_gc *gc)
{
	int			i;
	int			last_end;
	t_lexer_ctx	ctx;

	i = 0;
	last_end = -1;
	*token_list = NULL;
	ctx.line = line;
	ctx.i = &i;
	ctx.list = token_list;
	ctx.gc = gc;
	ctx.last_end = &last_end;
	while (line[i])
	{
		if (is_space(line[i]))
		{
			i++;
			continue ;
		}
		lexer_next_token(&ctx);
	}
}
