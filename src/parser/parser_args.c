/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:48:37 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/22 10:00:26 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"

static int	collect_single_arg(t_parser_ctx *ctx, char ***args,
		t_args_info *info, t_parser_command *cmd)
{
	if (cmd->argc >= info->capacity - 1 && !expand_args_array(ctx, args, info,
			cmd->argc))
	{
		*args = NULL;
		return (0);
	}
	(*args)[cmd->argc] = gc_strdup(ctx->gc, ctx->current->value);
	if (!(*args)[cmd->argc])
	{
		*args = NULL;
		return (0);
	}
	(cmd->argc)++;
	advance_token(ctx);
	return (1);
}

int	handle_mixed_tokens(t_parser_ctx *ctx, char ***args, t_args_info *info,
		t_parser_command *cmd)
{
	while (ctx->current)
	{
		if (ctx->current->type == TOKEN_WORD
			|| ctx->current->type == TOKEN_STRING)
		{
			if (!collect_single_arg(ctx, args, info, cmd))
				return (0);
		}
		else if (ctx->current->type >= TOKEN_REDIRECT_OUT
			&& ctx->current->type <= TOKEN_HEREDOC)
		{
			cmd->redirects = parse_redirections(ctx);
			if (ctx->error)
			{
				*args = NULL;
				return (0);
			}
		}
		else
			break ;
	}
	return (1);
}
