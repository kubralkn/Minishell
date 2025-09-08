/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_token_parsing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:27:00 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/22 09:57:41 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "lexer.h"

int	has_incomplete_heredoc_tokens(t_token *token_list)
{
	t_token	*current;
	int		has_heredoc;
	int		has_delimiter;

	has_heredoc = 0;
	has_delimiter = 0;
	current = token_list;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
		{
			has_heredoc = 1;
			if (current->next && (current->next->type == TOKEN_WORD
					|| current->next->type == TOKEN_STRING))
				has_delimiter = 1;
		}
		current = current->next;
	}
	return (has_heredoc && has_delimiter);
}

static int	count_heredoc_tokens(t_token *token_list)
{
	t_token	*current;
	int		count;

	count = 0;
	current = token_list;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
			count++;
		current = current->next;
	}
	return (count);
}

static char	*build_complete_delimiter(t_context *ctx, t_token **current)
{
	char	*delimiter;
	char	*temp;

	*current = (*current)->next;
	delimiter = gc_strdup(&ctx->gc, (*current)->value);
	*current = (*current)->next;
	while (*current && (*current)->is_adjacent)
	{
		temp = gc_strjoin(&ctx->gc, delimiter, (*current)->value);
		gc_free(&ctx->gc, delimiter);
		delimiter = temp;
		*current = (*current)->next;
	}
	return (delimiter);
}

char	**get_all_heredoc_delimiters(t_context *ctx, t_token *token_list)
{
	t_token	*current;
	char	**delimiters;
	int		count;
	int		i;

	count = count_heredoc_tokens(token_list);
	if (count == 0)
		return (NULL);
	delimiters = (char **)gc_alloc(&ctx->gc, sizeof(char *) * (count + 1));
	if (!delimiters)
		return (NULL);
	current = token_list;
	i = 0;
	while (current && i < count)
	{
		if (current->type == TOKEN_HEREDOC && current->next)
		{
			delimiters[i] = build_complete_delimiter(ctx, &current);
			i++;
		}
		else
			current = current->next;
	}
	delimiters[count] = NULL;
	return (delimiters);
}
