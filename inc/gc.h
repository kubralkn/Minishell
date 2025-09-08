/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:33:45 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/14 16:19:12 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GC_H
# define GC_H

# include <stdlib.h>

typedef struct s_gc_node
{
	void				*ptr;
	struct s_gc_node	*next;
}						t_gc_node;

typedef struct s_gc
{
	t_gc_node			*head;
	size_t				count;
}						t_gc;

void	gc_init(t_gc *gc);
void	gc_clear_all(t_gc *gc);
void	*gc_alloc(t_gc *gc, size_t size);
char	*gc_strdup(t_gc *gc, const char *str);
char	*gc_strjoin(t_gc *gc, const char *s1, const char *s2);
char	*gc_strjoin_char(t_gc *gc, const char *s1, char c);
char	*gc_readline(t_gc *gc, const char *prompt);
void	*gc_register(t_gc *gc, void *ptr);
void	gc_free(t_gc *gc, void *ptr);
char	**gc_split(t_gc *gc, const char *str, char delimiter);
char	*gc_substr(t_gc *gc, const char *str,
			unsigned int start, size_t len);
char	*gc_strtrim(t_gc *gc, const char *str, const char *set);
void	gc_free_string_array(t_gc *gc, char **array);

#endif
