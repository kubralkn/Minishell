/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:48:47 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/22 10:01:10 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "utils.h"

void	set_error(t_parser_ctx *ctx, char *msg)
{
	ctx->error = 1;
	ctx->error_msg = msg;
	print_error_message("babyshell🎀", NULL, msg);
}

int	init_command_node(t_parser_ctx *ctx, t_ast_node **cmd_node,
		t_parser_command **cmd)
{
	*cmd_node = create_ast_node(AST_COMMAND, ctx->gc);
	if (!*cmd_node)
		return (set_error(ctx, "Memory allocation failed"), 0);
	*cmd = create_command(ctx->gc);
	if (!*cmd)
	{
		return (set_error(ctx, "Memory allocation failed"), 0);
	}
	(*cmd_node)->command = *cmd;
	return (1);
}
