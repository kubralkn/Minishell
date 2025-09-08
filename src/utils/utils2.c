/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:15:23 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/22 10:05:26 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include "utils.h"

int	is_valid_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

char	*get_env_value(t_env_node *head, const char *key)
{
	t_env_node	*curr;

	curr = head;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			if (curr->value && ft_strcmp(curr->value,
					"__EXPORTED_WITHOUT_VALUE__") == 0)
				return (NULL);
			return (curr->value);
		}
		curr = curr->next;
	}
	return (NULL);
}

void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	print_error_message(const char *cmd, const char *arg, const char *msg)
{
	if (!cmd)
	{
		ft_putstr_fd("babyshell🎀: ", 2);
	}
	else
	{
		ft_putstr_fd((char *)cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	if (arg)
	{
		ft_putstr_fd((char *)arg, 2);
		ft_putstr_fd(": ", 2);
	}
	if (msg)
		ft_putstr_fd((char *)msg, 2);
	ft_putchar_fd('\n', 2);
}
