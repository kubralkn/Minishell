/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:15:00 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/22 10:03:47 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"

static void	remove_empty_tokens(t_token **token_list)
{
	t_token	*current;
	t_token	*next;
	t_token	*first_token;

	current = *token_list;
	first_token = current;
	while (current)
	{
		next = current->next;
		if (current->type == TOKEN_WORD && current->value
			&& ft_strlen(current->value) == 0 && current->from_expansion
			&& current != first_token)
		{
			if (current->prev)
				current->prev->next = current->next;
			else
				*token_list = current->next;
			if (current->next)
				current->next->prev = current->prev;
		}
		current = next;
	}
}

static void	remove_quote_marker_from_token(t_token *token, t_token *first_token)
{
	char	*clean_value;

	if (!token->value)
		return ;
	clean_value = ft_strnstr(token->value, "__EMPTY_QUOTES__",
			ft_strlen(token->value));
	if (clean_value)
	{
		if (!(token == first_token && ft_strlen(token->value) == 16))
		{
			while (clean_value)
			{
				ft_memmove(clean_value, clean_value + 16,
					ft_strlen(clean_value + 16) + 1);
				clean_value = ft_strnstr(token->value,
						"__EMPTY_QUOTES__", ft_strlen(token->value));
			}
		}
	}
}

static void	clean_empty_quote_markers(t_token **token_list)
{
	t_token	*current;
	t_token	*first_token;

	current = *token_list;
	first_token = current;
	while (current)
	{
		remove_quote_marker_from_token(current, first_token);
		current = current->next;
	}
}

void	clean_empty_quote_tokens(t_token **token_list)
{
	clean_empty_quote_markers(token_list);
	remove_empty_tokens(token_list);
}
