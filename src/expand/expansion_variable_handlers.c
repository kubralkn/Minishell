/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_variable_handlers.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:59:39 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/30 13:06:57 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "libft.h"
#include "utils.h"

char	*expand_exit_status(t_gc *gc, char *result, int exit_status)
{
	char	*exit_str;
	char	*temp;

	exit_str = ft_itoa(exit_status);
	if (!exit_str)
		return (NULL);
	temp = gc_strjoin(gc, result, exit_str);
	free(exit_str);
	if (!temp)
		return (NULL);
	return (temp);
}

static char	*handle_normal_variable(t_gc *gc, char *result,
										t_expand_context *ctx)
{
	char	*var_name;
	char	*env_value;
	char	*temp;

	var_name = extract_var_name(gc, ctx->str + ctx->i + 1);
	if (!var_name)
		return (NULL);
	env_value = get_env_value(ctx->shell->env_list, var_name);
	if (!env_value)
		env_value = "";
	temp = gc_strjoin(gc, result, env_value);
	if (!temp)
		return (NULL);
	ctx->i += ft_strlen(var_name) + 1;
	return (temp);
}

char	*handle_numeric_variable(t_gc *gc, char *result, t_expand_context *ctx)
{
	int		j;
	char	*temp;
	char	*remaining_digits;

	j = ctx->i + 1;
	while (ctx->str[j] && ft_isdigit(ctx->str[j]))
		j++;
	if (j > ctx->i + 2)
	{
		remaining_digits = gc_substr(gc, ctx->str, ctx->i + 2,
				j - (ctx->i + 2));
		if (!remaining_digits)
			return (NULL);
		temp = gc_strjoin(gc, result, remaining_digits);
	}
	else
		temp = gc_strdup(gc, result);
	if (!temp)
		return (NULL);
	ctx->i = j;
	return (temp);
}

char	*expand_variable(t_gc *gc, char *result, t_expand_context *ctx)
{
	if (ft_isdigit(ctx->str[ctx->i + 1]))
		return (handle_numeric_variable(gc, result, ctx));
	return (handle_normal_variable(gc, result, ctx));
}
