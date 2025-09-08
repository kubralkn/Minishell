/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adjacent_merger.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:47:40 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/22 09:59:49 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"

static int	is_adjacent_merge(t_token *a, t_token *b)
{
	return (b->is_adjacent && (a->type == TOKEN_WORD || a->type == TOKEN_STRING)
		&& (b->type == TOKEN_WORD || b->type == TOKEN_STRING));
}

static void	merge_token_values(t_token *a, t_token *b, t_gc *gc)
{
	char	*str;
	size_t	len_a;
	size_t	len_b;

	len_a = ft_strlen(a->value);
	len_b = ft_strlen(b->value);
	if (len_a == 0)
	{
		a->value = gc_strdup(gc, b->value);
	}
	else
	{
		str = gc_alloc(gc, len_a + len_b + 1);
		if (!str)
			return ;
		ft_strlcpy(str, a->value, len_a + len_b + 1);
		ft_strlcat(str, b->value, len_a + len_b + 1);
		a->value = str;
	}
}

static void	merge_tokens(t_token **list, t_gc *gc)
{
	t_token	*a;
	t_token	*b;

	a = *list;
	b = a->next;
	merge_token_values(a, b, gc);
	a->type = b->type;
	a->next = b->next;
	if (b->next)
		b->next->prev = a;
}

void	merge_adjacent_tokens(t_token **list, t_gc *gc)
{
	t_token	*cur;

	if (!list || !*list)
		return ;
	cur = *list;
	while (cur && cur->next)
	{
		if (is_adjacent_merge(cur, cur->next))
			merge_tokens(&cur, gc);
		else
			cur = cur->next;
	}
}
