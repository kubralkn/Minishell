/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:33:36 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/30 13:12:20 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "gc.h"

typedef struct s_shell			t_shell;

typedef struct s_env_node
{
	char						*key;
	char						*value;
	struct s_env_node			*next;
}								t_env_node;

typedef struct s_heredoc_content
{
	char						*content;
	struct s_heredoc_content	*next;
}								t_heredoc_content;

typedef struct s_context
{
	t_env_node					*env;
	t_gc						gc;
	char						*current_heredoc_content;
	t_heredoc_content			*heredoc_list;
	int							heredoc_quoted;
	t_shell						*shell;
	int							in_pipe;
}								t_context;

t_env_node						*env_find(t_context *ctx, const char *key);
void							env_assign(t_context *ctx, const char *key,
									const char *value);
void							env_initialize(t_context *ctx, char **envp);
char							**env_to_array(t_context *ctx);
int								chr_position(const char *str, char target);
void							remove_env_var(t_context *ctx, const char *key);
void							setup_shlvl(t_context *ctx);

#endif
