/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:33:50 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/30 13:06:57 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "gc.h"

typedef struct s_context	t_context;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_STRING,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_IN,
	TOKEN_APPEND,
	TOKEN_HEREDOC
}							t_token_type;

typedef struct s_token
{
	t_token_type			type;
	char					*value;
	int						is_adjacent;
	int						from_expansion;
	struct s_token			*next;
	struct s_token			*prev;
	t_gc					*gc;
}							t_token;

typedef struct s_quote_state
{
	int						in_single_quote;
	int						in_double_quote;
}							t_quote_state;

typedef struct s_lexer_ctx
{
	const char				*line;
	int						*i;
	t_token					**list;
	t_gc					*gc;
	int						*last_end;
}							t_lexer_ctx;

void	lexer(const char *line, t_token **token_list,
			t_gc *gc);
void	add_token(t_token **list, t_token_type type,
			char *value, t_gc *gc);
void	add_token_ctx(t_lexer_ctx *ctx, t_token_type type,
			char *value, int is_adjacent);
int		handle_pipe(t_lexer_ctx *ctx);
int		handle_quotes(t_lexer_ctx *ctx, int is_adjacent);
int		handle_redirect(t_lexer_ctx *ctx);
int		handle_dollar_quotes(t_lexer_ctx *ctx,
			int is_adjacent);
int		is_space(char c);
int		is_special_char(char c);
char	*get_quoted_string(const char *line, int *index, t_gc *gc);
char	*get_word(const char *line, int *index, t_gc *gc);
int		validate_token_syntax(t_token *token_list);
int		validate_quotes(const char *line,
			char *missing_quote_type);
void	merge_adjacent_tokens(t_token **token_list,
			t_gc *gc);

#endif
