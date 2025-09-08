/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_create.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:48:39 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/14 14:48:39 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"

t_ast_node	*create_ast_node(t_ast_type type, t_gc *gc)
{
	t_ast_node	*node;

	node = gc_alloc(gc, sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->left = NULL;
	node->right = NULL;
	node->command = NULL;
	return (node);
}

t_parser_command	*create_command(t_gc *gc)
{
	t_parser_command	*cmd;

	cmd = gc_alloc(gc, sizeof(t_parser_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->argc = 0;
	cmd->redirects = NULL;
	return (cmd);
}

t_redirect	*create_redirect(t_redirect_type type, char *filename, t_gc *gc)
{
	t_redirect	*redirect;

	redirect = gc_alloc(gc, sizeof(t_redirect));
	if (!redirect)
		return (NULL);
	redirect->type = type;
	redirect->filename = gc_strdup(gc, filename);
	redirect->heredoc_index = -1;
	redirect->next = NULL;
	if (!redirect->filename)
		return (NULL);
	return (redirect);
}

void	advance_token(t_parser_ctx *ctx)
{
	if (ctx->current)
		ctx->current = ctx->current->next;
}
