/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:58:17 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/14 15:58:17 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "executor.h"
#include "exit_codes.h"
#include "libft.h"
#include "utils.h"

static int	check_env_args(t_exec_command *exec_cmd)
{
	if (exec_cmd->argv[1])
	{
		ft_putstr_fd("env: '", 2);
		ft_putstr_fd(exec_cmd->argv[1], 2);
		ft_putstr_fd("': No such file or directory\n", 2);
		return (EXIT_COMMAND_NOT_FOUND);
	}
	return (EXIT_SUCCESS);
}

static void	print_env_variable(t_env_node *env, t_exec_command *exec_cmd)
{
	if (ft_strcmp(env->key, "_") == 0)
		ft_putstr_fd("_=/usr/bin/env\n", exec_cmd->fd_out);
	else
	{
		ft_putstr_fd(env->key, exec_cmd->fd_out);
		ft_putstr_fd("=", exec_cmd->fd_out);
		ft_putstr_fd(env->value, exec_cmd->fd_out);
		ft_putstr_fd("\n", exec_cmd->fd_out);
	}
}

static void	print_all_env_variables(t_context *ctx, t_exec_command *exec_cmd)
{
	t_env_node	*env;

	env = ctx->env;
	while (env != NULL)
	{
		if (env->value != NULL
			&& ft_strcmp(env->value, "__EXPORTED_WITHOUT_VALUE__") != 0)
		{
			print_env_variable(env, exec_cmd);
		}
		env = env->next;
	}
}

int	built_env(t_context *ctx, t_exec_command *exec_cmd)
{
	int	result;

	result = check_env_args(exec_cmd);
	if (result != EXIT_SUCCESS)
		return (result);
	print_all_env_variables(ctx, exec_cmd);
	return (EXIT_SUCCESS);
}
