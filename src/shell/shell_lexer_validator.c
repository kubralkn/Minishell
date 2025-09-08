/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_lexer_validator.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:54:03 by kalkan            #+#    #+#             */
/*   Updated: 2025/09/01 11:30:52 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "executor.h"
#include "exit_codes.h"
#include "lexer.h"
#include "libft.h"

int	handle_lexer_stage(char *line, t_token **token_list,
		char *missing_quote_type, t_gc *gc)
{
	int	syntax_result;

	if (!validate_quotes(line, missing_quote_type))
		return (4);
	lexer(line, token_list, gc);
	if (!*token_list)
		return (EXIT_GENERAL_ERROR);
	syntax_result = validate_token_syntax(*token_list);
	if (syntax_result == 0)
	{
		return (EXIT_SYNTAX_ERROR);
	}
	return (EXIT_SUCCESS);
}

void	cleanup_resources(t_context *ctx, char *line, t_token **token_list,
		t_ast_node **ast)
{
	if (line)
	{
		gc_free(&ctx->gc, line);
		line = NULL;
	}
	if (token_list && *token_list)
		*token_list = NULL;
	if (ast && *ast)
		*ast = NULL;
	ctx->heredoc_list = NULL;
}

int	handle_quote_errors(char *line, t_context *ctx, t_shell *shell,
	char missing_quote_type)
{
	if (missing_quote_type == '\'')
	{
		ft_putstr_fd(
			"babyshell🎀: unexpected EOF while looking for matching `''\n", 2);
		ft_putstr_fd(
			"babyshell🎀: syntax error: unexpected end of file\n", 2);
	}
	else if (missing_quote_type == '"')
	{
		ft_putstr_fd(
			"babyshell🎀: unexpected EOF while looking for matching `\"'\n", 2);
		ft_putstr_fd(
			"babyshell🎀: syntax error: unexpected end of file\n", 2);
	}
	shell->last_exit_status = EXIT_SYNTAX_ERROR;
	cleanup_resources(ctx, line, NULL, NULL);
	return (0);
}

int	handle_lexer_and_validation(char *line, t_context *ctx, t_shell *shell,
		t_token **token_list)
{
	int		lexer_result;
	char	missing_quote_type;

	lexer_result = 0;
	missing_quote_type = 0;
	*token_list = NULL;
	lexer_result = handle_lexer_stage(line, token_list, &missing_quote_type,
			&ctx->gc);
	if (lexer_result == 4 && !handle_quote_errors(line, ctx, shell,
			missing_quote_type))
		return (0);
	if (lexer_result != EXIT_SUCCESS)
	{
		if (lexer_result == EXIT_SYNTAX_ERROR)
			shell->last_exit_status = EXIT_SYNTAX_ERROR;
		else if (lexer_result == EXIT_GENERAL_ERROR)
			shell->last_exit_status = EXIT_GENERAL_ERROR;
		return (0);
	}
	return (1);
}
