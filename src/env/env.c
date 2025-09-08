/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:16:33 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/30 13:21:03 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include "utils.h"

static t_env_node	*env_create(t_context *ctx, const char *key,
		const char *value)
{
	t_env_node	*node;

	node = gc_alloc(&ctx->gc, sizeof(t_env_node));
	if (!node)
		return (NULL);
	node->key = gc_strdup(&ctx->gc, key);
	node->value = gc_strdup(&ctx->gc, value);
	if (!node->key || (value && !node->value))
	{
		if (node->key)
			gc_free(&ctx->gc, node->key);
		if (node->value)
			gc_free(&ctx->gc, node->value);
		gc_free(&ctx->gc, node);
		return (NULL);
	}
	node->next = ctx->env;
	ctx->env = node;
	return (node);
}

t_env_node	*env_find(t_context *ctx, const char *key)
{
	t_env_node	*current;

	current = ctx->env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	env_assign(t_context *ctx, const char *key, const char *value)
{
	t_env_node	*result;

	result = env_find(ctx, key);
	if (!result)
		env_create(ctx, key, value);
	else
	{
		if (result->value)
			gc_free(&ctx->gc, result->value);
		result->value = gc_strdup(&ctx->gc, value);
	}
}

void	env_initialize(t_context *ctx, char **envp)
{
	char	*copy;
	int		pos;

	ctx->env = NULL;
	while (*envp)
	{
		copy = gc_strdup(&ctx->gc, *envp);
		if (!copy)
		{
			envp++;
			continue ;
		}
		pos = chr_position(copy, '=');
		if (pos > 0)
		{
			copy[pos] = '\0';
			env_assign(ctx, copy, copy + pos + 1);
		}
		gc_free(&ctx->gc, copy);
		envp++;
	}
	setup_shlvl(ctx);
}
