/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_delimiter_processing.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:26:52 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/27 09:57:56 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"

static char	*append_char_to_delimiter(t_context *ctx, char *delimiter, char c)
{
	char	temp_str[2];
	char	*new_delimiter;

	temp_str[0] = c;
	temp_str[1] = '\0';
	new_delimiter = gc_strjoin(&ctx->gc, delimiter, temp_str);
	gc_free(&ctx->gc, delimiter);
	return (new_delimiter);
}

char	*process_delimiter_quotes(t_context *ctx, const char *delimiter)
{
	char	*final_delimiter;
	int		i;
	int		len;
	char	quote_char;

	final_delimiter = gc_strdup(&ctx->gc, "");
	i = 0;
	len = ft_strlen(delimiter);
	while (i < len)
	{
		if (delimiter[i] == '"' || delimiter[i] == '\'')
		{
			quote_char = delimiter[i];
			i++;
			while (i < len && delimiter[i] != quote_char)
				final_delimiter = append_char_to_delimiter(ctx, final_delimiter,
						delimiter[i++]);
			if (i < len && delimiter[i] == quote_char)
				i++;
		}
		else
			final_delimiter = append_char_to_delimiter(ctx, final_delimiter,
					delimiter[i++]);
	}
	return (final_delimiter);
}
