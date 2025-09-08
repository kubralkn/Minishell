/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_args_process.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:59:13 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/25 19:51:19 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include "parser.h"
#include "utils.h"
#include "libft.h"

static int	split_args(t_gc *gc, t_parser_command *cmd, char **words,
		int arg_index)
{
	char	**new_args;
	int		old_argc;
	int		word_count;

	word_count = 0;
	while (words[word_count])
		word_count++;
	old_argc = cmd->argc;
	cmd->argc = old_argc - 1 + word_count;
	new_args = allocate_new_args(gc, cmd->argc);
	if (!new_args)
		return (-1);
	copy_args_to_new_array(new_args, cmd, words, arg_index);
	cmd->args = new_args;
	return (word_count - 1);
}

static int	handle_expanded_arg(t_parser_command *cmd, char *expanded,
		int arg_index, t_gc *gc)
{
	char	**words;
	int		word_count;
	char	*original_arg;

	original_arg = cmd->args[arg_index];
	if (expanded[0] == '\0' || ft_strcmp(expanded, "__EMPTY_QUOTES__") == 0)
		return (handle_empty_expansion(cmd, expanded, arg_index, gc));
	if (should_skip_splitting(expanded, original_arg))
	{
		cmd->args[arg_index] = expanded;
		return (0);
	}
	words = split_and_count(gc, expanded, &word_count);
	if (!words)
		return (-1);
	return (split_args(gc, cmd, words, arg_index));
}

char	*handle_tilde_expansion(char *processed_str, t_shell *shell, t_gc *gc)
{
	char	*after_home;

	after_home = expand_home(processed_str, shell, gc);
	return (after_home);
}

char	*expand_string(const char *str, t_shell *shell, t_gc *gc)
{
	char	*processed_str;

	if (ft_strncmp(str, "__DQUOTED__", 11) == 0)
		return (gc_strdup(gc, str));
	if (str[0] == '~')
	{
		processed_str = gc_strdup(gc, str);
		if (!processed_str)
			return (NULL);
		return (handle_tilde_expansion(processed_str, shell, gc));
	}
	return (gc_strdup(gc, str));
}

int	expand_arg(t_parser_command *cmd, int arg_index, t_shell *shell, t_gc *gc)
{
	char	*expanded;

	expanded = expand_string(cmd->args[arg_index], shell, gc);
	if (!expanded)
		return (-1);
	expanded = clean_literal_markers(expanded, gc);
	if (!expanded)
		return (-1);
	return (handle_expanded_arg(cmd, expanded, arg_index, gc));
}
