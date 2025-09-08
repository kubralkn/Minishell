/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_env_basic.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:59:19 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/25 11:00:05 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "utils.h"
#include "libft.h"

static char	*handle_basic_expansions(t_gc *gc, char *result,
										t_expand_context *ctx, char next_char)
{
	if (next_char == '?')
	{
		ctx->i += 2;
		return (expand_exit_status(gc, result, ctx->shell->last_exit_status));
	}
	return (NULL);
}

static char	*expand_dollar_type(t_gc *gc, char *result, t_expand_context *ctx,
									char next_char)
{
	char	*temp;

	temp = handle_basic_expansions(gc, result, ctx, next_char);
	if (temp)
		return (temp);
	if (next_char == '_')
	{
		ctx->i += 2;
		if (ctx->shell->last_arg)
			temp = gc_strjoin(gc, result, ctx->shell->last_arg);
		else
			temp = gc_strjoin(gc, result, "");
		if (!temp)
			return (NULL);
		return (temp);
	}
	if (is_valid_char(next_char) || ft_isdigit(next_char))
		return (expand_variable(gc, result, ctx));
	temp = gc_strjoin_char(gc, result, ctx->str[ctx->i++]);
	return (temp);
}

static char	*handle_dollar_expansion(t_gc *gc, char *result,
										t_expand_context *ctx)
{
	if (ctx->str[ctx->i] != '$')
		return (NULL);
	return (expand_dollar_type(gc, result, ctx, ctx->str[ctx->i + 1]));
}

char	*process_character(t_gc *gc, char *result, t_expand_context *ctx)
{
	char	*temp;

	if (ft_strncmp(&ctx->str[ctx->i], "__LITERAL__", 11) == 0)
	{
		ctx->i += 11;
		while (ctx->str[ctx->i] && ft_strncmp(&ctx->str[ctx->i],
				"__LITERAL__", 11) != 0)
		{
			temp = gc_strjoin_char(gc, result, ctx->str[ctx->i++]);
			if (!temp)
				return (NULL);
			result = temp;
		}
		return (result);
	}
	temp = handle_dollar_expansion(gc, result, ctx);
	if (temp)
		return (temp);
	temp = gc_strjoin_char(gc, result, ctx->str[ctx->i++]);
	return (temp);
}
