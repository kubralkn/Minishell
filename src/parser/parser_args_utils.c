/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_args_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:48:33 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/22 10:00:06 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"
#include "parser.h"

int	expand_args_array(t_parser_ctx *ctx, char ***args, t_args_info *info,
		int current_argc)
{
	char	**new_args;
	int		i;

	info->capacity *= 2;
	new_args = gc_alloc(ctx->gc, sizeof(char *) * (info->capacity));
	if (!new_args)
		return (set_error(ctx, "Memory allocation failed"), 0);
	i = -1;
	while (++i < current_argc)
		new_args[i] = (*args)[i];
	*args = new_args;
	return (1);
}

char	**init_args_array(t_parser_ctx *ctx, t_args_info *info)
{
	char	**args;

	info->capacity = 4;
	args = gc_alloc(ctx->gc, sizeof(char *) * (info->capacity));
	if (!args)
		set_error(ctx, "Memory allocation failed");
	return (args);
}
