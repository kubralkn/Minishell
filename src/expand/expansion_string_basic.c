/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_string_basic.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:59:32 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/25 19:51:58 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "expander.h"
#include "utils.h"
#include "executor.h"

char	*expand_home(const char *str, t_shell *shell, t_gc *gc)
{
	char	*home;
	char	*result;

	if (str[0] == '~' && (str[1] == '/' || str[1] == '\0'))
	{
		home = get_env_value(shell->env_list, "HOME");
		if (!home || !*home)
			return (gc_strdup(gc, str));
		result = gc_strjoin(gc, home, str + 1);
		return (result);
	}
	return (gc_strdup(gc, str));
}

static char	*clean_literal_marker(char *str)
{
	char	*pos;
	size_t	str_len;
	size_t	marker_len;

	marker_len = 11;
	str_len = ft_strlen(str);
	pos = ft_strnstr(str, "__LITERAL__", str_len);
	while (pos)
	{
		ft_memmove(pos, pos + marker_len, ft_strlen(pos + marker_len) + 1);
		str_len = ft_strlen(str);
		pos = ft_strnstr(str, "__LITERAL__", str_len);
	}
	return (str);
}

static char	*clean_dquoted_marker(char *str)
{
	char	*pos;
	size_t	str_len;
	size_t	marker_len;

	marker_len = 11;
	str_len = ft_strlen(str);
	pos = ft_strnstr(str, "__DQUOTED__", str_len);
	while (pos)
	{
		ft_memmove(pos, pos + marker_len, ft_strlen(pos + marker_len) + 1);
		str_len = ft_strlen(str);
		pos = ft_strnstr(str, "__DQUOTED__", str_len);
	}
	return (str);
}

char	*clean_literal_markers(const char *str, t_gc *gc)
{
	char	*result;

	if (!str)
		return (NULL);
	result = gc_strdup(gc, str);
	if (!result)
		return (NULL);
	result = clean_literal_marker(result);
	result = clean_dquoted_marker(result);
	return (result);
}
