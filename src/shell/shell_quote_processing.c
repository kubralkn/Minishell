/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_quote_processing.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:54:08 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/22 10:02:51 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"

static int	process_single_quoted_string(t_token *token, t_gc *gc)
{
	char	*literal_content;
	char	*prefixed_content;
	size_t	len;

	len = ft_strlen(token->value);
	if (token->value[0] == '\'' && len > 1 && token->value[len - 1] == '\'')
	{
		literal_content = gc_substr(gc, token->value, 1, len - 2);
		if (!literal_content || ft_strlen(literal_content) == 0)
		{
			token->value = gc_strdup(gc, "__EMPTY_QUOTES__");
			return (1);
		}
		prefixed_content = gc_alloc(gc, ft_strlen(literal_content) + 12);
		if (!prefixed_content)
			return (0);
		ft_strlcpy(prefixed_content, "__LITERAL__", 12);
		ft_strlcat(prefixed_content, literal_content, ft_strlen(literal_content)
			+ 12);
		token->value = prefixed_content;
		token->from_expansion = 1;
		return (1);
	}
	return (0);
}

static int	process_empty_double_quotes(t_token *token, t_gc *gc)
{
	size_t	len;

	len = ft_strlen(token->value);
	if (token->value[0] == '"' && len == 2 && token->value[1] == '"')
	{
		token->value = gc_strdup(gc, "__EMPTY_QUOTES__");
		return (1);
	}
	return (0);
}

static int	process_double_quoted_string(t_token *token, t_gc *gc)
{
	char	*literal_content;
	char	*prefixed_content;
	size_t	len;

	len = ft_strlen(token->value);
	if (token->value[0] == '"' && len > 2 && token->value[len - 1] == '"')
	{
		literal_content = gc_substr(gc, token->value, 1, len - 2);
		if (!literal_content)
			return (0);
		prefixed_content = gc_alloc(gc, ft_strlen(literal_content) + 12);
		if (!prefixed_content)
			return (0);
		ft_strlcpy(prefixed_content, "__DQUOTED__", 12);
		ft_strlcat(prefixed_content, literal_content, ft_strlen(literal_content)
			+ 12);
		token->value = prefixed_content;
		return (1);
	}
	return (0);
}

int	process_string_token(t_token *token, t_gc *gc)
{
	if (token->type != TOKEN_STRING)
		return (0);
	if (process_single_quoted_string(token, gc))
		return (1);
	if (process_double_quoted_string(token, gc))
		return (1);
	if (process_empty_double_quotes(token, gc))
		return (1);
	return (0);
}
