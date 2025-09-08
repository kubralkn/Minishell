/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:29:43 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/22 09:58:16 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static t_token_type	is_redirect_type(const char current_char,
		const char next_char)
{
	if (current_char == '>')
	{
		if (next_char == '>')
			return (TOKEN_APPEND);
		return (TOKEN_REDIRECT_OUT);
	}
	else if (current_char == '<')
	{
		if (next_char == '<')
			return (TOKEN_HEREDOC);
		return (TOKEN_REDIRECT_IN);
	}
	return (0);
}

static int	get_redirect_info(const char *line, int i, t_token_type *type,
		int *len)
{
	char	current_char;
	char	next_char;

	current_char = line[i];
	next_char = line[i + 1];
	*type = is_redirect_type(current_char, next_char);
	if (*type == TOKEN_APPEND || *type == TOKEN_HEREDOC)
		*len = 2;
	else if (*type == TOKEN_REDIRECT_OUT || *type == TOKEN_REDIRECT_IN)
		*len = 1;
	else
		return (0);
	return (1);
}

int	handle_redirect(t_lexer_ctx *ctx)
{
	t_token_type	type;
	int				len;
	char			*value;

	if (!get_redirect_info(ctx->line, *ctx->i, &type, &len))
		return (0);
	if (type == TOKEN_APPEND)
		value = ">>";
	else if (type == TOKEN_HEREDOC)
		value = "<<";
	else if (type == TOKEN_REDIRECT_OUT)
		value = ">";
	else
		value = "<";
	add_token_ctx(ctx, type, value, 0);
	*ctx->i += len;
	return (1);
}
