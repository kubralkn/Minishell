/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:33:43 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/25 19:39:16 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

typedef struct s_gc				t_gc;
typedef struct s_parser_command	t_parser_command;
typedef struct s_redirect		t_redirect;
typedef struct s_shell			t_shell;
typedef struct s_context		t_context;

typedef struct s_expand_context
{
	const char					*str;
	int							i;
	t_shell						*shell;
	char						quote;
	t_gc						*gc;
}								t_expand_context;

char	*expand_env_variables(const char *str,
			t_shell *shell, t_gc *gc);
int		expand_command_args(t_parser_command *cmd,
			t_shell *shell, t_gc *gc);
int		expand_redirects(t_parser_command *cmd,
			t_context *ctx);
char	*expand_variable(t_gc *gc, char *result,
			t_expand_context *ctx);
char	*expand_exit_status(t_gc *gc, char *result,
			int exit_status);
char	*expand_double_quote_content(const char *str,
			t_shell *shell, t_gc *gc);
char	*expand_string(const char *str, t_shell *shell,
			t_gc *gc);
char	*expand_home(const char *str, t_shell *shell,
			t_gc *gc);
int		should_skip_splitting(char *expanded,
			char *original_arg);
char	**allocate_new_args(t_gc *gc, int new_argc);
void	copy_args_to_new_array(char **new_args,
			t_parser_command *cmd, char **words,
			int arg_index);
char	*handle_numeric_variable(t_gc *gc, char *result,
			t_expand_context *ctx);
char	*process_expansion_loop(t_gc *gc, char *result,
			t_expand_context *ctx, int str_len);
int		handle_empty_expansion(t_parser_command *cmd,
			char *expanded, int arg_index, t_gc *gc);
int		process_export_unset_args(t_parser_command *cmd,
			t_shell *shell, t_gc *gc);
int		process_regular_args(t_parser_command *cmd,
			t_shell *shell, t_gc *gc);
int		expand_arg(t_parser_command *cmd, int arg_index,
			t_shell *shell, t_gc *gc);
char	*process_character(t_gc *gc, char *result,
			t_expand_context *ctx);
char	*clean_literal_markers(const char *str,
			t_gc *gc);
char	*handle_tilde_expansion(char *processed_str,
			t_shell *shell, t_gc *gc);
char	*wrap_literal_quotes(t_gc *gc, const char *str);

#endif
