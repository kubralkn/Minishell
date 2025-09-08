/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_env_variables.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:59:21 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/22 09:52:23 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "expander.h"

static char	*process_env_expansion(char *cleaned_str, t_shell *shell, t_gc *gc)
{
	char				*result;
	char				*temp;
	t_expand_context	ctx;

	result = gc_strdup(gc, "");
	if (!result)
		return (NULL);
	ctx.str = cleaned_str;
	ctx.i = 0;
	ctx.shell = shell;
	ctx.gc = gc;
	while (cleaned_str[ctx.i])
	{
		temp = process_character(gc, result, &ctx);
		if (!temp)
			return (NULL);
		result = temp;
	}
	return (result);
}

char	*expand_env_variables(const char *str, t_shell *shell, t_gc *gc)
{
	char	*result;

	result = process_env_expansion((char *)str, shell, gc);
	return (result);
}
