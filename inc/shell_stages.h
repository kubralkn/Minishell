/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_stages.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:33:55 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/14 15:52:51 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_STAGES_H
# define SHELL_STAGES_H

typedef struct s_context	t_context;
typedef struct s_shell		t_shell;
typedef struct s_token		t_token;
typedef struct s_ast_node	t_ast_node;
typedef struct s_gc			t_gc;

typedef struct s_execution_data
{
	t_context	*ctx;
	t_shell		*shell;
}	t_execution_data;

void	process_shell_line(char *line, t_context *ctx, t_shell *shell);

int		handle_lexer_stage(char *line, t_token **token_list,
			char *missing_quote_type, t_gc *gc);
int		handle_lexer_and_validation(char *line, t_context *ctx,
			t_shell *shell, t_token **token_list);
int		handle_parser_stage(t_token *token_list, t_ast_node **ast,
			t_gc *gc);
void	cleanup_resources(t_context *ctx, char *line,
			t_token **token_list, t_ast_node **ast);
int		handle_quote_errors(char *line, t_context *ctx, t_shell *shell,
			char missing_quote_type);
int		handle_expansion_and_merge(t_token **token_list, t_shell *shell,
			t_gc *gc);
int		handle_token_expansion(t_token **token_list, t_shell *shell, t_gc *gc);
int		process_string_token(t_token *token, t_gc *gc);
#endif