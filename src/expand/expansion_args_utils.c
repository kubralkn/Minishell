/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_args_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:59:15 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/22 09:51:14 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser.h"
#include "utils.h"

static int	is_fully_quoted(const char *str)
{
	int		len;
	char	quote_char;

	len = ft_strlen(str);
	if (len < 2)
		return (0);
	quote_char = str[0];
	if (quote_char != '"' && quote_char != '\'')
		return (0);
	if (str[len - 1] == quote_char)
		return (1);
	return (0);
}

static int	was_originally_quoted(const char *str)
{
	if (ft_strncmp(str, "__LITERAL__", 11) == 0)
		return (1);
	if (ft_strncmp(str, "__DQUOTED__", 11) == 0)
		return (1);
	return (is_fully_quoted(str));
}

int	should_skip_splitting(char *expanded, char *original_arg)
{
	char	*double_space;

	if (was_originally_quoted(original_arg))
		return (1);
	if (!ft_strchr(expanded, ' '))
		return (1);
	if (is_whitespace_only(expanded))
		return (1);
	double_space = ft_strnstr(expanded, "  ", ft_strlen(expanded));
	if (double_space)
		return (1);
	return (0);
}

char	**allocate_new_args(t_gc *gc, int new_argc)
{
	char	**new_args;

	new_args = gc_alloc(gc, sizeof(char *) * (new_argc + 1));
	if (!new_args)
	{
		return (NULL);
	}
	return (new_args);
}

void	copy_args_to_new_array(char **new_args, t_parser_command *cmd,
								char **words, int arg_index)
{
	int	i;
	int	j;
	int	word_count;
	int	old_argc;

	old_argc = cmd->argc + 1;
	word_count = 0;
	while (words[word_count])
		word_count++;
	i = -1;
	while (++i < arg_index)
		new_args[i] = cmd->args[i];
	j = 0;
	while (j < word_count)
	{
		new_args[i + j] = words[j];
		j++;
	}
	j = arg_index + 1;
	while (j < old_argc - word_count)
	{
		new_args[i + word_count + j - arg_index - 1] = cmd->args[j];
		j++;
	}
	new_args[cmd->argc] = NULL;
}
