/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:33:33 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/30 13:06:56 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "gc.h"

typedef struct s_exec_command	t_exec_command;
typedef struct s_context		t_context;
typedef struct s_shell			t_shell;
typedef struct s_env_node		t_env_node;

int		is_builtin_command(char *cmd);
int		execute_builtin(t_exec_command *exec_cmd,
			t_shell *shell, t_context *ctx);

int		built_cd(t_context *ctx,
			t_exec_command *command);
int		built_echo(t_exec_command *command);
int		built_env(t_context *ctx,
			t_exec_command *exec_cmd);
int		built_exit(t_exec_command *com, t_shell *shell, t_context *ctx);
int		built_export(t_context *ctx, char **args);
int		built_pwd(t_context *ctx);
int		built_unset(t_context *ctx, char **args);

char	*handle_quoted_arg(char *arg, t_gc *gc);
int		validate_numeric_string(char *final_trimmed);
int		check_exit_arg(char *arg, int *exit_code, t_gc *gc);

int		is_valid_name(const char *str);
int		split_env_var(t_context *ctx, char *arg,
			char **key_out, char **value_out);

#endif
