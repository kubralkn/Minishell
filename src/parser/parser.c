/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:48:50 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/14 14:48:50 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"

t_ast_node	*parse(t_token *tokens, t_gc *gc)
{
	t_parser_ctx	ctx;
	t_ast_node		*ast;

	if (!tokens)
		return (NULL);
	ctx.tokens = tokens;
	ctx.current = tokens;
	ctx.error = 0;
	ctx.error_msg = NULL;
	ctx.heredoc_count = 0;
	ctx.gc = gc;
	ast = parse_pipeline(&ctx);
	if (ctx.error)
		return (NULL);
	return (ast);
}

t_ast_node	*parse_pipeline(t_parser_ctx *ctx)
{
	t_ast_node	*left;

	left = parse_command(ctx);
	if (!left || ctx->error)
		return (left);
	while (ctx->current && ctx->current->type == TOKEN_PIPE)
	{
		left = parse_single_pipe(ctx, left);
		if (!left || ctx->error)
			return (left);
	}
	return (left);
}

t_ast_node	*parse_command(t_parser_ctx *ctx)
{
	t_ast_node			*cmd_node;
	t_parser_command	*cmd;
	char				**args;
	t_args_info			info;

	if (!ctx->current)
		return (NULL);
	if (!init_command_node(ctx, &cmd_node, &cmd))
		return (NULL);
	args = init_args_array(ctx, &info);
	if (!args)
		return (NULL);
	if (!handle_mixed_tokens(ctx, &args, &info, cmd))
		return (NULL);
	args[cmd->argc] = NULL;
	cmd->args = args;
	if (cmd->argc == 0 && !cmd->redirects)
		return (set_error(ctx, "Empty command"), NULL);
	return (cmd_node);
}
