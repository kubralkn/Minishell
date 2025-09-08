/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:16:26 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/22 09:40:48 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "utils.h"
#include "env.h"

static int	count_env_var(t_env_node *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->value && ft_strcmp(env->value,
				"__EXPORTED_WITHOUT_VALUE__") != 0)
			count++;
		env = env->next;
	}
	return (count);
}

static char	*make_env_string(t_context *ctx, const char *key, const char *value)
{
	size_t	klen;
	size_t	vlen;
	char	*entry;

	klen = ft_strlen(key);
	vlen = ft_strlen(value);
	entry = gc_alloc(&ctx->gc, klen + vlen + 2);
	if (!entry)
		return (NULL);
	ft_strcpy(entry, key);
	entry[klen] = '=';
	ft_strcpy(entry + klen + 1, value);
	return (entry);
}

static char	**allocate_env_array(t_context *ctx, int *count)
{
	char	**arr;

	*count = count_env_var(ctx->env);
	arr = gc_alloc(&ctx->gc, sizeof(char *) * (*count + 1));
	return (arr);
}

static int	fill_env_array(t_context *ctx, char **arr, int count)
{
	t_env_node	*current;
	int			arr_index;

	current = ctx->env;
	arr_index = 0;
	while (current && arr_index < count)
	{
		if (current->value && ft_strcmp(current->value,
				"__EXPORTED_WITHOUT_VALUE__") != 0)
		{
			arr[arr_index] = make_env_string(ctx, current->key, current->value);
			if (!arr[arr_index])
				return (-1);
			arr_index++;
		}
		current = current->next;
	}
	arr[arr_index] = NULL;
	return (0);
}

char	**env_to_array(t_context *ctx)
{
	char	**arr;
	int		count;

	arr = allocate_env_array(ctx, &count);
	if (!arr)
		return (NULL);
	if (fill_env_array(ctx, arr, count) == -1)
		return (NULL);
	return (arr);
}
