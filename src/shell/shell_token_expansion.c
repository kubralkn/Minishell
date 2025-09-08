/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_token_expansion.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:54:10 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/22 10:03:30 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exit_codes.h"
#include "expander.h"
#include "shell_stages.h"
#include "utils.h"
#include "libft.h"

static int	process_double_quoted_expansion(t_token *current, t_shell *shell,
			t_gc *gc)
{
	char	*expanded;
	char	*marked;

	if (!current->value || ft_strncmp(current->value, "__DQUOTED__", 11) != 0)
		return (0);
	expanded = expand_double_quote_content(current->value + 11, shell, gc);
	if (!expanded)
		return (-1);
	if (ft_strlen(expanded) > 0)
	{
		marked = gc_alloc(gc, ft_strlen(expanded) + 12);
		if (marked)
		{
			ft_strlcpy(marked, "__DQUOTED__", 12);
			ft_strlcat(marked, expanded, ft_strlen(expanded) + 12);
			current->value = marked;
		}
		else
			current->value = expanded;
	}
	else
		current->value = gc_strdup(gc, "__DQUOTED__");
	current->type = TOKEN_WORD;
	current->from_expansion = 1;
	return (1);
}

static int	process_string_expansion(t_token *current, t_shell *shell, t_gc *gc)
{
	int	result;

	if (!process_string_token(current, gc))
		return (0);
	result = process_double_quoted_expansion(current, shell, gc);
	if (result == -1)
		return (-1);
	return (1);
}

static int	process_regular_expansion(t_token *current, t_shell *shell,
			t_gc *gc)
{
	char	*expanded;

	if (!current->value)
		return (-1);
	expanded = expand_env_variables(current->value, shell, gc);
	if (!expanded)
		return (-1);
	current->value = expanded;
	current->type = TOKEN_WORD;
	current->from_expansion = 1;
	return (1);
}

int	handle_token_expansion(t_token **token_list, t_shell *shell, t_gc *gc)
{
	t_token	*current;
	int		result;

	current = *token_list;
	while (current)
	{
		if ((current->type == TOKEN_WORD || current->type == TOKEN_STRING)
			&& !current->from_expansion)
		{
			result = process_string_expansion(current, shell, gc);
			if (result == -1)
				return (EXIT_SUCCESS);
			if (result == 1)
			{
				current = current->next;
				continue ;
			}
			result = process_regular_expansion(current, shell, gc);
			if (result == -1)
				return (EXIT_SUCCESS);
		}
		current = current->next;
	}
	return (EXIT_GENERAL_ERROR);
}

int	handle_expansion_and_merge(t_token **token_list, t_shell *shell, t_gc *gc)
{
	if (!handle_token_expansion(token_list, shell, gc))
		return (EXIT_SUCCESS);
	merge_adjacent_tokens(token_list, gc);
	clean_empty_quote_tokens(token_list);
	return (EXIT_GENERAL_ERROR);
}
