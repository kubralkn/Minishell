/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:20:00 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/30 13:06:57 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"
#include <sys/stat.h>

void	print_warning_message_with_line(const char *delimiter, int line_number)
{
	char	*line_str;

	ft_putstr_fd("babyshell🎀: warning: here-document at line ", 2);
	line_str = ft_itoa(line_number);
	if (line_str)
	{
		ft_putstr_fd(line_str, 2);
		free(line_str);
	}
	ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
	if (delimiter)
		ft_putstr_fd((char *)delimiter, 2);
	ft_putstr_fd("')", 2);
	ft_putchar_fd('\n', 2);
}

int	argument_count(char **argv)
{
	int	count;

	count = 0;
	while (argv[count])
		count++;
	return (count);
}

int	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == 0)
		return (S_ISDIR(path_stat.st_mode));
	return (0);
}

long long	ft_atoll(const char *str)
{
	long long	result;
	int			sign;
	int			i;

	result = 0;
	sign = 1;
	i = 0;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] && ft_isdigit(str[i]))
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}
