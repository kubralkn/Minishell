/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_loop_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:59:23 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/22 09:52:59 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include "libft.h"
#include "utils.h"
#include "executor.h"

static char	*append_regular_char(t_gc *gc, char *result, t_expand_context *ctx)
{
	char	*temp;

	temp = gc_strjoin_char(gc, result, ctx->str[ctx->i]);
	if (!temp)
		return (NULL);
	ctx->i++;
	return (temp);
}

static char	*handle_dollar_character(t_gc *gc, char *result,
										t_expand_context *ctx)
{
	char	*temp;

	if (ctx->str[ctx->i + 1] == '?')
	{
		ctx->i += 2;
		temp = expand_exit_status(gc, result, ctx->shell->last_exit_status);
	}
	else if (is_valid_char(ctx->str[ctx->i + 1])
		|| ft_isdigit(ctx->str[ctx->i + 1]))
		temp = expand_variable(gc, result, ctx);
	else
		temp = append_regular_char(gc, result, ctx);
	if (!temp)
		return (NULL);
	return (temp);
}

char	*process_expansion_loop(t_gc *gc, char *result, t_expand_context *ctx,
									int str_len)
{
	char	*temp;

	while (ctx->i < str_len)
	{
		if (ctx->str[ctx->i] == '$' && ctx->i + 1 < str_len)
		{
			temp = handle_dollar_character(gc, result, ctx);
			if (!temp)
				return (NULL);
			result = temp;
		}
		else
		{
			temp = append_regular_char(gc, result, ctx);
			if (!temp)
				return (NULL);
			result = temp;
		}
	}
	return (result);
}

char	*expand_double_quote_content(const char *str, t_shell *shell, t_gc *gc)
{
	t_expand_context	ctx;
	char				*result;
	int					str_len;
	char				*wrapped;

	if (ft_strlen(str) == 2 && str[0] == '\'' && str[1] == '\'')
		return (gc_strdup(gc, "__LITERAL__''"));
	ctx.i = 0;
	ctx.str = str;
	ctx.shell = shell;
	ctx.quote = '"';
	ctx.gc = gc;
	str_len = ft_strlen(str);
	result = gc_strdup(gc, "");
	if (!result)
		return (NULL);
	result = process_expansion_loop(gc, result, &ctx, str_len);
	if (!result)
		return (NULL);
	wrapped = wrap_literal_quotes(gc, result);
	return (wrapped);
}
