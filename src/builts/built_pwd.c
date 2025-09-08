/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:58:32 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/14 15:58:32 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "exit_codes.h"
#include "utils.h"
#include <stdio.h>

int	built_pwd(t_context *ctx)
{
	t_env_node	*pwd_env;

	pwd_env = env_find(ctx, "PWD");
	if (pwd_env && pwd_env->value)
	{
		printf("%s\n", pwd_env->value);
		return (EXIT_SUCCESS);
	}
	else
	{
		print_error_message("pwd", NULL, "PWD not set");
		return (EXIT_GENERAL_ERROR);
	}
}
