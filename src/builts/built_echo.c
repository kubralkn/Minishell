/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:58:15 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/14 15:58:15 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "exit_codes.h"
#include "libft.h"

static void	clean_echo_literal_markers(char *str)
{
	char	*pos;
	size_t	str_len;
	size_t	marker_len;

	marker_len = 11;
	if (!str)
		return ;
	str_len = ft_strlen(str);
	pos = ft_strnstr(str, "__LITERAL__", str_len);
	while (pos)
	{
		ft_memmove(pos, pos + marker_len, ft_strlen(pos + marker_len) + 1);
		str_len = ft_strlen(str);
		pos = ft_strnstr(str, "__LITERAL__", str_len);
	}
}

static int	echo_options(char **argv, int *line_flag)
{
	int		i;
	char	*p;

	i = 1;
	*line_flag = 1;
	while (argv[i])
	{
		p = argv[i];
		if (*p != '-')
			break ;
		p++;
		if (*p != 'n')
			break ;
		while (*p == 'n')
		{
			p++;
		}
		if (*p != '\0')
			break ;
		*line_flag = 0;
		i++;
	}
	return (i);
}

int	built_echo(t_exec_command *command)
{
	int		i;
	int		line_flag;
	char	**argv;

	argv = command->argv;
	i = echo_options(argv, &line_flag);
	while (argv[i])
	{
		clean_echo_literal_markers(argv[i]);
		ft_putstr_fd(argv[i], command->fd_out);
		if (argv[i + 1])
			ft_putchar_fd(' ', command->fd_out);
		i++;
	}
	if (line_flag)
		ft_putstr_fd("\n", command->fd_out);
	return (EXIT_SUCCESS);
}
