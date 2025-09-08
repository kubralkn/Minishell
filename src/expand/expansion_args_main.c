/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_args_main.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:59:10 by kalkan            #+#    #+#             */
/*   Updated: 2025/09/01 10:59:59 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "expander.h"
#include "utils.h"
#include "env.h"

int	process_regular_args(t_parser_command *cmd, t_shell *shell, t_gc *gc)
{
	int	i;
	int	result;

	i = 0;
	while (i < cmd->argc && cmd->args)
	{
		result = expand_arg(cmd, i, shell, gc);
		if (result == -1)
			return (-1);
		else if (result == -2)
			continue ;
		else
			i += result + 1;
	}
	return (0);
}

int	process_export_unset_args(t_parser_command *cmd, t_shell *shell, t_gc *gc)
{
	int		i;
	char	*expanded;

	i = 0;
	while (i < cmd->argc && cmd->args)
	{
		expanded = expand_string(cmd->args[i], shell, gc);
		if (!expanded)
			return (-1);
		expanded = clean_literal_markers(expanded, gc);
		if (!expanded)
			return (-1);
		cmd->args[i] = expanded;
		i++;
	}
	return (0);
}

static int	process_args_loop(t_parser_command *cmd, t_shell *shell,
								int is_export_or_unset, t_gc *gc)
{
	if (is_export_or_unset)
		return (process_export_unset_args(cmd, shell, gc));
	else
		return (process_regular_args(cmd, shell, gc));
}

int	expand_command_args(t_parser_command *cmd, t_shell *shell, t_gc *gc)
{
	int	is_export_or_unset;

	if (cmd->argc > 0
		&& cmd->args[0]
		&& (ft_strcmp(cmd->args[0], "export") == 0
			|| ft_strcmp(cmd->args[0], "unset") == 0))
		is_export_or_unset = 1;
	else
		is_export_or_unset = 0;
	return (process_args_loop(cmd, shell, is_export_or_unset, gc));
}

int	expand_redirects(t_parser_command *cmd, t_context *ctx)
{
	t_redirect	*redir;
	char		*expanded;

	redir = cmd->redirects;
	while (redir != NULL)
	{
		expanded = clean_literal_markers(redir->filename, &ctx->gc);
		if (!expanded)
			return (-1);
		redir->filename = expanded;
		redir = redir->next;
	}
	return (0);
}
