/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_args_empty.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:59:08 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/25 19:51:46 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"
#include "utils.h"

static int	remove_arg_and_shift(t_parser_command *cmd, int arg_index)
{
	int	j;

	j = arg_index;
	while (j < cmd->argc - 1)
	{
		cmd->args[j] = cmd->args[j + 1];
		j++;
	}
	cmd->args[cmd->argc - 1] = NULL;
	cmd->argc--;
	return (-2);
}

int	handle_empty_expansion(t_parser_command *cmd, char *expanded, int arg_index,
								t_gc *gc)
{
	if (arg_index != 0)
		return (0);
	if (ft_strcmp(expanded, "__EMPTY_QUOTES__") == 0)
	{
		cmd->args[0] = gc_strdup(gc, "");
		if (!cmd->args[0])
			return (-1);
		return (0);
	}
	if (expanded[0] == '\0')
	{
		if (cmd->argc > 1)
			return (remove_arg_and_shift(cmd, 0));
		cmd->argc = 0;
		cmd->args[0] = NULL;
		return (-2);
	}
	return (0);
}
