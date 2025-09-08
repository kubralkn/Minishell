/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:33:38 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/27 09:56:19 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <sys/types.h>

typedef struct s_env_node		t_env_node;
typedef struct s_context		t_context;
typedef struct s_redirect		t_redirect;
typedef struct s_parser_command	t_parser_command;
typedef struct s_ast_node		t_ast_node;

typedef struct s_exec_command
{
	char						**argv;
	int							fd_in;
	int							fd_out;
	int							fd_err;
}								t_exec_command;

typedef struct s_shell
{
	t_env_node					*env_list;
	int							last_exit_status;
	int							should_exit;
	char						*last_arg;
}								t_shell;

typedef struct s_redirect_info
{
	int							heredoc_fd;
	int							pipefd[2];
}								t_redirect_info;

typedef struct s_pipe_pids
{
	pid_t	pid1;
	pid_t	pid2;
	int		pipefd[2];
}			t_pipe_pids;

int		execute_ast(t_ast_node *ast, t_shell *shell, t_context *ctx);
int		execute_command(t_parser_command *cmd, t_shell *shell,
			t_context *ctx);
int		execute_external_command(t_exec_command *exec_cmd,
			t_shell *shell, t_context *ctx);
int		execute_builtin(t_exec_command *exec_cmd, t_shell *shell,
			t_context *ctx);
int		execute_command_logic(t_exec_command *exec_cmd, t_shell *shell,
			t_context *ctx);
int		execute_and_cleanup(t_exec_command *exec_cmd, t_shell *shell,
			t_context *ctx, int saved_stdout);
int		is_builtin_command(char *cmd);
void	cleanup_exec_command_fds(t_exec_command *exec_cmd);
pid_t	create_left_process(t_ast_node *ast, t_shell *shell, t_context *ctx,
			int pipefd[2]);
pid_t	create_right_process(t_ast_node *ast, t_shell *shell, t_context *ctx,
			int pipefd[2]);
char	*get_executable_path(char *cmd_name, t_context *ctx, t_shell *shell);
int		handle_single_redirect(t_context *ctx, t_redirect *redirect,
			t_exec_command *exec_cmd, t_shell *shell);
int		execute_pipe(t_ast_node *ast, t_shell *shell, t_context *ctx);
void	init_exec_command(t_exec_command *exec_cmd, t_parser_command *cmd);
char	*find_command_path(char *cmd, t_context *ctx);

#endif
