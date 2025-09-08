/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:33:48 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/25 19:38:59 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# include "executor.h"
# include "parser.h"

int		has_incomplete_heredoc_tokens(t_token *token_list);
char	**get_all_heredoc_delimiters(t_context *ctx, t_token *token_list);
char	*process_delimiter_quotes(t_context *ctx, const char *delimiter);
char	*handle_heredoc(t_context *ctx, const char *initial_line,
			t_token *token_list);
char	*read_heredoc_content(t_context *ctx, const char *delimiter);
int		handle_heredoc_fd_redirect(t_context *ctx, t_exec_command *exec_cmd,
			t_shell *shell, int heredoc_index);

#endif
