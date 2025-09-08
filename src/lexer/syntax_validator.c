/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:08:51 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/22 10:08:51 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "utils.h"

static int	check_pipe_at_start(t_token *token_list)
{
	if (token_list && token_list->type == TOKEN_PIPE)
	{
		print_error_message(NULL, NULL,
			"syntax error near unexpected token `|'");
		return (0);
	}
	return (1);
}

static int	validate_pipe_syntax(t_token *token_list)
{
	t_token	*current;

	if (!check_pipe_at_start(token_list))
		return (0);
	current = token_list;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (!current->next)
			{
				print_error_message(NULL, NULL,
					"syntax error near unexpected token `|'");
				return (0);
			}
			if (current->next->type == TOKEN_PIPE)
			{
				print_error_message(NULL, NULL,
					"syntax error near unexpected token `|'");
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}

static int	check_redirect_syntax(t_token *current)
{
	if (!current->next)
	{
		print_error_message(NULL, NULL,
			"syntax error near unexpected token `newline'");
		return (0);
	}
	if (current->next->type != TOKEN_WORD
		&& current->next->type != TOKEN_STRING)
	{
		print_error_message(NULL, NULL,
			"syntax error near unexpected token `newline'");
		return (0);
	}
	return (1);
}

static int	validate_redirect_syntax(t_token *token_list)
{
	t_token	*current;

	current = token_list;
	while (current)
	{
		if (current->type >= TOKEN_REDIRECT_OUT
			&& current->type <= TOKEN_HEREDOC)
		{
			if (!check_redirect_syntax(current))
				return (0);
		}
		current = current->next;
	}
	return (1);
}

int	validate_token_syntax(t_token *token_list)
{
	int	pipe_result;
	int	redirect_result;

	pipe_result = validate_pipe_syntax(token_list);
	if (pipe_result != 1)
		return (pipe_result);
	redirect_result = validate_redirect_syntax(token_list);
	return (redirect_result);
}
