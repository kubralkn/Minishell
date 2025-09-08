/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:48:42 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/22 10:00:52 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

t_ast_node	*create_pipe_node(t_ast_node *left, t_parser_ctx *ctx)
{
	t_ast_node	*pipe_node;

	pipe_node = create_ast_node(AST_PIPE, ctx->gc);
	if (!pipe_node)
	{
		set_error(ctx, "Memory allocation failed");
		return (NULL);
	}
	pipe_node->left = left;
	return (pipe_node);
}

t_ast_node	*parse_single_pipe(t_parser_ctx *ctx, t_ast_node *left)
{
	t_ast_node	*pipe_node;

	advance_token(ctx);
	pipe_node = create_pipe_node(left, ctx);
	if (!pipe_node)
		return (NULL);
	pipe_node->right = parse_command(ctx);
	if (!pipe_node->right || ctx->error)
	{
		return (NULL);
	}
	return (pipe_node);
}
