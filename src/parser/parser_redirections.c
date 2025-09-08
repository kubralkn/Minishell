/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:48:44 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/14 14:48:44 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "parser.h"

static void	parse_type_redirect(char *value, t_redirect_type *type)
{
	if (ft_strncmp(value, ">>", 2) == 0)
		*type = REDIR_APPEND;
	else if (ft_strncmp(value, "<<", 2) == 0)
		*type = REDIR_HEREDOC;
	else if (value[0] == '>')
		*type = REDIR_OUT;
	else if (value[0] == '<')
		*type = REDIR_IN;
	else
		*type = REDIR_OUT;
}

static t_redirect	*parse_single_redirection(t_parser_ctx *ctx)
{
	t_redirect_type		redir_type;
	char				*filename;
	t_redirect			*current;

	parse_type_redirect(ctx->current->value, &redir_type);
	advance_token(ctx);
	if (!ctx->current || (ctx->current->type != TOKEN_WORD
			&& ctx->current->type != TOKEN_STRING))
		return (set_error(ctx, "Expected filename"), NULL);
	filename = ctx->current->value;
	current = create_redirect(redir_type, filename, ctx->gc);
	if (!current)
	{
		set_error(ctx, "Memory allocation failed");
		return (NULL);
	}
	if (redir_type == REDIR_HEREDOC)
	{
		current->heredoc_index = ctx->heredoc_count;
		ctx->heredoc_count++;
	}
	advance_token(ctx);
	return (current);
}

t_redirect	*parse_redirections(t_parser_ctx *ctx)
{
	t_redirect	*redirects;
	t_redirect	*current;
	t_redirect	*last;

	last = NULL;
	redirects = NULL;
	while (ctx->current && (ctx->current->type >= TOKEN_REDIRECT_OUT
			&& ctx->current->type <= TOKEN_HEREDOC))
	{
		current = parse_single_redirection(ctx);
		if (!current)
			break ;
		if (!redirects)
			redirects = current;
		else
			last->next = current;
		last = current;
	}
	return (redirects);
}
