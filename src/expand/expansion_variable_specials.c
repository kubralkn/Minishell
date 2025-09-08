/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_variable_specials.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:59:41 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/22 09:55:43 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"

static int	count_quotes(const char *str)
{
	int	len;
	int	quote_count;
	int	i;

	len = ft_strlen(str);
	quote_count = 0;
	i = 0;
	while (i < len)
	{
		if (str[i] == '\'')
			quote_count++;
		i++;
	}
	return (quote_count);
}

static char	*build_wrapped_string(t_gc *gc, const char *str, int len,
									int quote_count)
{
	char	*result;
	int		result_len;
	int		i;
	int		j;

	result_len = len + (quote_count * 11) + 1;
	result = gc_alloc(gc, result_len);
	if (!result)
		return (NULL);
	j = 0;
	i = 0;
	while (i < len)
	{
		if (str[i] == '\'')
		{
			ft_strlcpy(result + j, "__LITERAL__'", result_len - j);
			j += 12;
		}
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

char	*wrap_literal_quotes(t_gc *gc, const char *str)
{
	int	len;
	int	quote_count;

	len = ft_strlen(str);
	quote_count = count_quotes(str);
	if (quote_count == 0)
		return (gc_strdup(gc, str));
	return (build_wrapped_string(gc, str, len, quote_count));
}
