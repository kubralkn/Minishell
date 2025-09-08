/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:29:38 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/30 13:06:57 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"

int	handle_pipe(t_lexer_ctx *ctx)
{
	if (ctx->line[*ctx->i] == '|')
	{
		add_token_ctx(ctx, TOKEN_PIPE, "|", 0);
		(*ctx->i)++;
		return (1);
	}
	return (0);
}

int	handle_quotes(t_lexer_ctx *ctx, int is_adjacent)
{
	char	*quoted_str;

	if (ctx->line[*ctx->i] == '\'' || ctx->line[*ctx->i] == '"')
	{
		quoted_str = get_quoted_string(ctx->line, ctx->i, ctx->gc);
		if (quoted_str)
			add_token_ctx(ctx, TOKEN_STRING, quoted_str, is_adjacent);
		return (1);
	}
	return (0);
}

int	handle_dollar_quotes(t_lexer_ctx *ctx, int is_adjacent)
{
	char	*quoted_str;
	char	*content;
	int		len;

	if (ctx->line[*ctx->i] == '$' && (ctx->line[*ctx->i + 1] == '"'
			|| ctx->line[*ctx->i + 1] == '\''))
	{
		(*ctx->i)++;
		quoted_str = get_quoted_string(ctx->line, ctx->i, ctx->gc);
		if (quoted_str && ft_strlen(quoted_str) >= 2)
		{
			len = ft_strlen(quoted_str) - 2;
			content = gc_substr(ctx->gc, quoted_str, 1, len);
			add_token_ctx(ctx, TOKEN_STRING, content, is_adjacent);
		}
		return (1);
	}
	return (0);
}
