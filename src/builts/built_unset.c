/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:58:35 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/14 15:58:35 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

int	built_unset(t_context *ctx, char **args)
{
	int	i;

	i = 1;
	if (!args[1])
		return (EXIT_SUCCESS);
	while (args[i])
	{
		remove_env_var(ctx, args[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}
