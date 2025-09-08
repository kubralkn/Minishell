/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:29:45 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/22 09:58:37 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static t_token	*create_token(t_token_type type, char *value, t_gc *gc)
{
	t_token	*new_token;

	new_token = gc_alloc(gc, sizeof(t_token));
	new_token->type = type;
	new_token->value = gc_strdup(gc, value);
	new_token->next = NULL;
	new_token->prev = NULL;
	new_token->is_adjacent = 0;
	new_token->from_expansion = 0;
	new_token->gc = gc;
	return (new_token);
}

void	add_token(t_token **list, t_token_type type, char *value, t_gc *gc)
{
	t_token	*node;
	t_token	*new_token;

	new_token = create_token(type, value, gc);
	if (!new_token)
		return ;
	if (*list == NULL)
	{
		*list = new_token;
		return ;
	}
	node = *list;
	while (node->next != NULL)
		node = node->next;
	node->next = new_token;
	new_token->prev = node;
}

void	add_token_ctx(t_lexer_ctx *ctx, t_token_type type, char *value,
			int is_adjacent)
{
	t_token	*last;

	add_token(ctx->list, type, value, ctx->gc);
	if (*ctx->list)
	{
		last = *ctx->list;
		while (last->next)
			last = last->next;
		last->is_adjacent = is_adjacent;
	}
}
