/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2025/08/14 15:58:11 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/14 15:58:11 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "executor.h"
#include "exit_codes.h"
#include "utils.h"
#include <stdio.h>
#include <unistd.h>

static char	*get_dest_dir(t_context *ctx, t_exec_command *command)
{
	char		*target_dir;
	t_env_node	*home_env;

	if (!command || !command->argv)
		return (NULL);
	if (command->argv[1] != NULL)
		target_dir = command->argv[1];
	else
	{
		home_env = env_find(ctx, "HOME");
		if (home_env)
			target_dir = home_env->value;
		else
			target_dir = NULL;
	}
	return (target_dir);
}

static int	update_pwd(t_context *ctx)
{
	char		*old_pwd;
	char		*new_pwd;
	t_env_node	*pwd_env;

	pwd_env = env_find(ctx, "PWD");
	if (pwd_env)
		old_pwd = pwd_env->value;
	else
		old_pwd = "";
	new_pwd = getcwd(NULL, 0);
	if (new_pwd == NULL)
	{
		print_error_message("cd", NULL,
			"error getting current directory: getcwd: cannot access "
			"parent directories: No such file or directory");
		env_assign(ctx, "OLDPWD", old_pwd);
		env_assign(ctx, "PWD", "");
		return (EXIT_SUCCESS);
	}
	env_assign(ctx, "OLDPWD", old_pwd);
	env_assign(ctx, "PWD", new_pwd);
	free(new_pwd);
	return (EXIT_SUCCESS);
}

static int	validate_and_change_dir(char *target_dir)
{
	if (access(target_dir, F_OK) != 0)
	{
		print_error_message("cd", target_dir, "No such file or directory");
		return (EXIT_GENERAL_ERROR);
	}
	if (access(target_dir, X_OK) != 0)
	{
		print_error_message("cd", target_dir, "Permission denied");
		return (EXIT_GENERAL_ERROR);
	}
	if (access(target_dir, R_OK) != 0)
	{
		print_error_message("cd", target_dir, "Permission denied");
		return (EXIT_GENERAL_ERROR);
	}
	if (chdir(target_dir) < 0)
	{
		print_error_message("cd", target_dir, "No such file or directory");
		return (EXIT_GENERAL_ERROR);
	}
	return (EXIT_SUCCESS);
}

int	built_cd(t_context *ctx, t_exec_command *command)
{
	char	*target_dir;
	int		result;

	if (!command || !command->argv)
		return (EXIT_GENERAL_ERROR);
	if (command->argv[1] && command->argv[2])
	{
		print_error_message("cd", NULL, "too many arguments");
		return (EXIT_GENERAL_ERROR);
	}
	target_dir = get_dest_dir(ctx, command);
	if (!target_dir)
	{
		print_error_message("cd", NULL, "HOME not set");
		return (EXIT_GENERAL_ERROR);
	}
	result = validate_and_change_dir(target_dir);
	if (result != EXIT_SUCCESS)
		return (result);
	return (update_pwd(ctx));
}
