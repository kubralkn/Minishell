/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_string.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:59:56 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/14 15:59:57 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"
#include "libft.h"

char	**gc_split(t_gc *gc, const char *str, char delimiter)
{
	char	**result;
	int		i;

	if (!gc)
		return (ft_split(str, delimiter));
	result = ft_split(str, delimiter);
	if (!result)
		return (NULL);
	gc_register(gc, result);
	i = 0;
	while (result[i])
	{
		gc_register(gc, result[i]);
		i++;
	}
	return (result);
}

char	*gc_strjoin(t_gc *gc, const char *s1, const char *s2)
{
	char	*result;

	if (!gc)
		return (ft_strjoin(s1, s2));
	result = ft_strjoin(s1, s2);
	if (!result)
		return (NULL);
	gc_register(gc, result);
	return (result);
}

char	*gc_substr(t_gc *gc, const char *str, unsigned int start, size_t len)
{
	char	*result;

	if (!gc)
		return (ft_substr(str, start, len));
	result = ft_substr(str, start, len);
	if (!result)
		return (NULL);
	gc_register(gc, result);
	return (result);
}

char	*gc_strtrim(t_gc *gc, const char *str, const char *set)
{
	char	*result;

	if (!gc)
		return (ft_strtrim(str, set));
	result = ft_strtrim(str, set);
	if (!result)
		return (NULL);
	gc_register(gc, result);
	return (result);
}

char	*gc_strjoin_char(t_gc *gc, const char *str, char c)
{
	char	*char_str;
	char	*result;

	if (!gc)
	{
		char_str = malloc(2);
		if (!char_str)
			return (NULL);
		char_str[0] = c;
		char_str[1] = '\0';
		result = ft_strjoin(str, char_str);
		free(char_str);
		return (result);
	}
	char_str = gc_alloc(gc, 2);
	if (!char_str)
		return (NULL);
	char_str[0] = c;
	char_str[1] = '\0';
	result = gc_strjoin(gc, str, char_str);
	return (result);
}
