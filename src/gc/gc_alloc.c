/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_alloc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:59:52 by kalkan            #+#    #+#             */
/*   Updated: 2025/09/01 12:03:01 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"
#include "libft.h"
#include <readline/readline.h>
#include <readline/history.h>

void	*gc_alloc(t_gc *gc, size_t size)
{
	void	*ptr;
	void	*registered;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	registered = gc_register(gc, ptr);
	return (registered);
}

char	*gc_readline(t_gc *gc, const char *prompt)
{
	char	*line;
	char	*registered;

	line = readline(prompt);
	if (!line)
		return (NULL);
	add_history(line);
	registered = gc_register(gc, line);
	return (registered);
}

void	gc_free_string_array(t_gc *gc, char **array)
{
	int	i;

	if (!array || !gc)
		return ;
	i = 0;
	while (array[i])
	{
		gc_free(gc, array[i]);
		i++;
	}
	gc_free(gc, array);
}

char	*gc_strdup(t_gc *gc, const char *str)
{
	char	*result;
	char	*registered;

	if (!gc)
		return (ft_strdup(str));
	result = ft_strdup(str);
	if (!result)
		return (NULL);
	registered = gc_register(gc, result);
	return (registered);
}
