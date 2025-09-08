/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:33:53 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/14 15:33:53 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

typedef enum e_token_type	t_token_type;
typedef struct s_token		t_token;
typedef struct s_gc			t_gc;

typedef enum e_ast_type
{
	AST_COMMAND,
	AST_PIPE,
}	t_ast_type;

typedef enum e_redirect_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redirect_type;

typedef struct s_args_info
{
	int		capacity;
}	t_args_info;

typedef struct s_redirect
{
	t_redirect_type		type;
	char				*filename;
	int					heredoc_index;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_parser_command
{
	char		**args;
	int			argc;
	t_redirect	*redirects;
}	t_parser_command;

typedef struct s_ast_node
{
	t_ast_type	type;
	union
	{
		t_parser_command	*command;
		struct
		{
			struct s_ast_node	*left;
			struct s_ast_node	*right;
		};
	};
}	t_ast_node;

typedef struct s_parser_ctx
{
	t_token	*tokens;
	t_token	*current;
	int		error;
	char	*error_msg;
	int		heredoc_count;
	t_gc	*gc;
}	t_parser_ctx;

t_ast_node			*parse(t_token *tokens, t_gc *gc);
t_ast_node			*parse_pipeline(t_parser_ctx *ctx);
t_ast_node			*parse_command(t_parser_ctx *ctx);
t_redirect			*parse_redirections(t_parser_ctx *ctx);
t_ast_node			*parse_single_pipe(t_parser_ctx *ctx, t_ast_node *left);
int					init_command_node(t_parser_ctx *ctx, t_ast_node **cmd_node,
						t_parser_command **cmd);
char				**init_args_array(t_parser_ctx *ctx, t_args_info *info);
int					handle_mixed_tokens(t_parser_ctx *ctx, char ***args,
						t_args_info *info, t_parser_command *cmd);
void				set_error(t_parser_ctx *ctx, char *msg);
int					expand_args_array(t_parser_ctx *ctx, char ***args,
						t_args_info *info, int current_argc);
t_ast_node			*create_ast_node(t_ast_type type, t_gc *gc);
t_parser_command	*create_command(t_gc *gc);
t_redirect			*create_redirect(t_redirect_type type, char *filename,
						t_gc *gc);
void				advance_token(t_parser_ctx *ctx);

#endif
