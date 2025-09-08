/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:16:30 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/30 13:17:43 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "gc.h"
#include "libft.h"
#include "utils.h"

int	chr_position(const char *str, char target)
{
	int	position;

	if (!str)
		return (-1);
	position = 0;
	while (str[position] && str[position] != target)
		position++;
	return (position);
}

void	remove_env_var(t_context *ctx, const char *key)
{
	t_env_node	*curr;
	t_env_node	*prev;

	curr = ctx->env;
	prev = NULL;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			if (prev == NULL)
				ctx->env = curr->next;
			else
				prev->next = curr->next;
			gc_free(&ctx->gc, curr->key);
			if (curr->value)
				gc_free(&ctx->gc, curr->value);
			gc_free(&ctx->gc, curr);
			return ;
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
}

static int	get_shlvl_value(t_context *ctx)
{
	t_env_node	*shlvl_node;
	int			shlvl_value;

	shlvl_node = env_find(ctx, "SHLVL");
	if (shlvl_node && shlvl_node->value)
	{
		shlvl_value = ft_atoi(shlvl_node->value);
		if (shlvl_value < 0)
			shlvl_value = 0;
		shlvl_value++;
	}
	else
		shlvl_value = 1;
	return (shlvl_value);
}

static void	set_shlvl_env(t_context *ctx, int shlvl_value)
{
	char		*shlvl_str;
	char		*temp_str;

	shlvl_str = gc_strdup(&ctx->gc, "");
	{
		temp_str = ft_itoa(shlvl_value);
		if (temp_str)
		{
			shlvl_str = gc_strdup(&ctx->gc, temp_str);
			free(temp_str);
		}
	}
	if (shlvl_str && ft_strlen(shlvl_str) > 0)
		env_assign(ctx, "SHLVL", shlvl_str);
}

void	setup_shlvl(t_context *ctx)
{
	int	shlvl_value;

	shlvl_value = get_shlvl_value(ctx);
	set_shlvl_env(ctx, shlvl_value);
}
