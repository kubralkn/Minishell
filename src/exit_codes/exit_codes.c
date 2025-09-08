/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_codes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:25:18 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/22 09:49:26 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "exit_codes.h"
#include <sys/stat.h>
#include <unistd.h>

int	normalize_exit_code(int exit_code)
{
	if (exit_code < 0)
		return (256 + (exit_code % 256));
	return (exit_code % 256);
}

void	set_exit_status(t_shell *shell, int exit_code)
{
	if (shell)
		shell->last_exit_status = normalize_exit_code(exit_code);
}

int	get_command_exit_code(char *cmd_path)
{
	struct stat	st;

	if (!cmd_path)
		return (EXIT_COMMAND_NOT_FOUND);
	if (access(cmd_path, F_OK) != 0)
		return (EXIT_COMMAND_NOT_FOUND);
	if (stat(cmd_path, &st) == 0 && S_ISDIR(st.st_mode))
		return (EXIT_CANNOT_EXECUTE);
	if (access(cmd_path, X_OK) != 0)
		return (EXIT_CANNOT_EXECUTE);
	return (EXIT_SUCCESS);
}

static int	get_exit_code_from_signal(int signal_num)
{
	return (128 + signal_num);
}

int	handle_process_exit_code(int status)
{
	int	signal_num;

	if ((status & 0x7F) == 0)
		return ((status >> 8) & 0xFF);
	if ((status & 0x7F) != 0)
	{
		signal_num = status & 0x7F;
		return (get_exit_code_from_signal(signal_num));
	}
	return (EXIT_GENERAL_ERROR);
}
