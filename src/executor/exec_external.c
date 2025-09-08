/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_external.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:58:50 by kalkan            #+#    #+#             */
/*   Updated: 2025/09/01 10:58:41 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "executor.h"
#include "exit_codes.h"
#include "libft.h"
#include "utils.h"
#include <sys/stat.h>
#include <unistd.h>

static int	validate_file_access(char *cmd_name, t_shell *shell)
{
	if (access(cmd_name, F_OK) != 0)
	{
		print_error_message(NULL, cmd_name, "No such file or directory");
		set_exit_status(shell, EXIT_COMMAND_NOT_FOUND);
		return (EXIT_COMMAND_NOT_FOUND);
	}
	if (access(cmd_name, X_OK) != 0)
	{
		print_error_message(NULL, cmd_name, "Permission denied");
		set_exit_status(shell, EXIT_CANNOT_EXECUTE);
		return (EXIT_CANNOT_EXECUTE);
	}
	return (EXIT_SUCCESS);
}

static int	check_directory_path(char *cmd_name, t_shell *shell)
{
	struct stat	st;

	if (stat(cmd_name, &st) == 0 && S_ISDIR(st.st_mode))
	{
		if (ft_strchr(cmd_name, '/') != NULL)
		{
			print_error_message(NULL, cmd_name, "Is a directory");
			set_exit_status(shell, EXIT_CANNOT_EXECUTE);
		}
		else
		{
			print_error_message(NULL, cmd_name, "command not found");
			set_exit_status(shell, EXIT_COMMAND_NOT_FOUND);
		}
		return (EXIT_GENERAL_ERROR);
	}
	return (EXIT_SUCCESS);
}

static char	*handle_absolute_path(char *cmd_name, t_context *ctx,
		t_shell *shell)
{
	int		validation_result;
	char	*result;

	validation_result = validate_file_access(cmd_name, shell);
	if (validation_result != EXIT_SUCCESS)
		return (NULL);
	if (check_directory_path(cmd_name, shell) != EXIT_SUCCESS)
		return (NULL);
	result = gc_strdup(&ctx->gc, cmd_name);
	return (result);
}

static char	*handle_relative_path(char *cmd_name, t_context *ctx,
		t_shell *shell)
{
	char	*cmd_path;
	int		exit_code;
	char	*path_env;

	path_env = get_env_value(ctx->env, "PATH");
	cmd_path = find_command_path(cmd_name, ctx);
	exit_code = get_command_exit_code(cmd_path);
	if (exit_code != EXIT_SUCCESS)
	{
		if (exit_code == EXIT_COMMAND_NOT_FOUND)
		{
			if (!path_env)
				print_error_message(NULL, cmd_name,
					"No such file or directory");
			else
				print_error_message(NULL, cmd_name, "command not found");
		}
		else if (exit_code == EXIT_CANNOT_EXECUTE)
			print_error_message(NULL, cmd_name, "Permission denied");
		set_exit_status(shell, exit_code);
		return (NULL);
	}
	return (cmd_path);
}

char	*get_executable_path(char *cmd_name, t_context *ctx, t_shell *shell)
{
	int	has_slash;

	if (!cmd_name || !*cmd_name)
		return (set_exit_status(shell, EXIT_SUCCESS), NULL);
	if (ft_strcmp(cmd_name, ".") == 0)
	{
		print_error_message(NULL, ".", "filename argument required");
		print_error_message(".", "usage", ". filename [arguments]");
		return (set_exit_status(shell, 2), NULL);
	}
	if (ft_strcmp(cmd_name, "..") == 0)
	{
		print_error_message(NULL, cmd_name, "command not found");
		set_exit_status(shell, EXIT_COMMAND_NOT_FOUND);
		return (NULL);
	}
	if (ft_strchr(cmd_name, '/') != NULL)
		has_slash = 1;
	else
		has_slash = 0;
	if (has_slash)
		return (handle_absolute_path(cmd_name, ctx, shell));
	else
		return (handle_relative_path(cmd_name, ctx, shell));
}
