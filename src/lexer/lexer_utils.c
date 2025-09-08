/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:29:48 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/14 14:29:48 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"
#include "libft.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	is_special_char(char c)
{
	return (c == '|' || c == '>' || c == '<' || c == '\'' || c == '"');
}

char	*get_quoted_string(const char *line, int *index, t_gc *gc)
{
	char	quote;
	int		start;

	quote = line[*index];
	start = *index;
	(*index)++;
	while (line[*index] && line[*index] != quote)
		(*index)++;
	if (line[*index] == quote)
		(*index)++;
	return (gc_substr(gc, line, start, *index - start));
}

char	*get_word(const char *line, int *index, t_gc *gc)
{
	int		start;
	int		i;
	int		len;
	char	*str;

	start = *index;
	i = start;
	while (line[i] && !is_space(line[i]) && !is_special_char(line[i]))
		i++;
	len = i - start;
	str = gc_substr(gc, line, start, len);
	*index = i - 1;
	return (str);
}
