/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_core.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:59:54 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/30 13:25:46 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"

void	gc_init(t_gc *gc)
{
	if (!gc)
		return ;
	gc->head = NULL;
	gc->count = 0;
}

static t_gc_node	*create_gc_node(void *ptr)
{
	t_gc_node	*node;

	node = malloc(sizeof(t_gc_node));
	if (!node)
		return (NULL);
	node->ptr = ptr;
	node->next = NULL;
	return (node);
}

void	*gc_register(t_gc *gc, void *ptr)
{
	t_gc_node	*node;
	t_gc_node	*current;

	if (!gc || !ptr)
		return (NULL);
	node = create_gc_node(ptr);
	if (!node)
	{
		free(ptr);
		return (NULL);
	}
	if (!gc->head)
		gc->head = node;
	else
	{
		current = gc->head;
		while (current->next)
			current = current->next;
		current->next = node;
	}
	gc->count++;
	return (ptr);
}

void	gc_free(t_gc *gc, void *ptr)
{
	t_gc_node	*current;
	t_gc_node	*prev;

	if (!gc || !ptr)
		return ;
	current = gc->head;
	prev = NULL;
	while (current)
	{
		if (current->ptr == ptr)
		{
			if (prev)
				prev->next = current->next;
			else
				gc->head = current->next;
			free(current->ptr);
			current->ptr = NULL;
			free(current);
			gc->count--;
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	gc_clear_all(t_gc *gc)
{
	t_gc_node	*current;
	t_gc_node	*next;

	if (!gc)
		return ;
	if (!gc->head)
	{
		gc->count = 0;
		return ;
	}
	current = gc->head;
	while (current)
	{
		next = current->next;
		if (current->ptr)
			free(current->ptr);
		free(current);
		current = next;
	}
	gc->head = NULL;
	gc->count = 0;
}
