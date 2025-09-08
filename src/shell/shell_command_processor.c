/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_command_processor.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:53:59 by kalkan            #+#    #+#             */
/*   Updated: 2025/09/01 11:31:19 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "exit_codes.h"
#include "heredoc.h"
#include "lexer.h"
#include "shell_stages.h"

static int	execute_command_line(t_ast_node *ast, t_execution_data *exec_data)
{
	int	exec_status;

	exec_status = execute_ast(ast, exec_data->shell, exec_data->ctx);
	exec_status = normalize_exit_code(exec_status);
	exec_data->shell->last_exit_status = exec_status;
	return (exec_status);
}

static int	process_expansion_and_parsing(char *line,
		t_execution_data *exec_data, t_token **token_list, t_ast_node **ast)
{
	if (!handle_expansion_and_merge(token_list, exec_data->shell,
			&exec_data->ctx->gc))
	{
		cleanup_resources(exec_data->ctx, line, token_list, ast);
		return (0);
	}
	if (!*token_list)
	{
		exec_data->shell->last_exit_status = EXIT_SUCCESS;
		cleanup_resources(exec_data->ctx, line, token_list, ast);
		return (0);
	}
	if (handle_parser_stage(*token_list, ast,
			&exec_data->ctx->gc) != EXIT_SUCCESS)
	{
		exec_data->shell->last_exit_status = EXIT_SYNTAX_ERROR;
		cleanup_resources(exec_data->ctx, line, token_list, ast);
		return (0);
	}
	return (1);
}

static char	*handle_heredoc_processing(char *line, t_context *ctx)
{
	t_token	*token_list;
	char	*complete_command;

	token_list = NULL;
	lexer(line, &token_list, &ctx->gc);
	if (has_incomplete_heredoc_tokens(token_list))
	{
		complete_command = handle_heredoc(ctx, line, token_list);
		if (!complete_command)
		{
			gc_free(&ctx->gc, line);
			return (NULL);
		}
		gc_free(&ctx->gc, line);
		return (complete_command);
	}
	else
		return (line);
}

void	process_shell_line(char *line, t_context *ctx, t_shell *shell)
{
	t_token				*token_list;
	t_ast_node			*ast;
	t_execution_data	exec_data;
	char				*processed_line;

	exec_data.ctx = ctx;
	exec_data.shell = shell;
	ast = NULL;
	token_list = NULL;
	processed_line = handle_heredoc_processing(line, ctx);
	if (!processed_line)
		return ;
	if (!handle_lexer_and_validation(processed_line, ctx, shell, &token_list))
	{
		cleanup_resources(ctx, processed_line, &token_list, &ast);
		return ;
	}
	if (!process_expansion_and_parsing(processed_line, &exec_data,
			&token_list, &ast))
	{
		cleanup_resources(ctx, processed_line, &token_list, &ast);
		return ;
	}
	execute_command_line(ast, &exec_data);
	cleanup_resources(ctx, processed_line, &token_list, &ast);
}
