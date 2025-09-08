/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:59:02 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/25 19:41:20 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include "utils.h"
#include <unistd.h>

static char	*create_full_path(t_context *ctx, char *dir, char *cmd)
{
	char	*path;
	int		len;

	len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	path = gc_alloc(&ctx->gc, len);
	if (!path)
		return (NULL);
	ft_strcpy(path, dir);
	ft_strcat(path, "/");
	ft_strcat(path, cmd);
	return (path);
}

static char	*handle_no_path_env(char *cmd, t_context *ctx)
{
	if (access(cmd, F_OK) == 0)
	{
		if (access(cmd, X_OK) == 0)
			return (gc_strdup(&ctx->gc, cmd));
		return (gc_strdup(&ctx->gc, cmd));
	}
	return (NULL);
}

static char	*search_in_path(char *cmd, t_context *ctx)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;

	path_env = get_env_value(ctx->env, "PATH");
	if (!path_env)
		return (handle_no_path_env(cmd, ctx));
	paths = gc_split(&ctx->gc, path_env, ':');
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		full_path = create_full_path(ctx, paths[i], cmd);
		if (full_path && access(full_path, X_OK) == 0)
		{
			gc_free_string_array(&ctx->gc, paths);
			return (full_path);
		}
		gc_free(&ctx->gc, full_path);
	}
	gc_free_string_array(&ctx->gc, paths);
	return (NULL);
}

char	*find_command_path(char *cmd, t_context *ctx)
{
	char	*path;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (gc_strdup(&ctx->gc, cmd));
		return (NULL);
	}
	path = search_in_path(cmd, ctx);
	return (path);
}
