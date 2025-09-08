/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_exit_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:58:25 by kalkan            #+#    #+#             */
/*   Updated: 2025/09/01 10:59:23 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exit_codes.h"
#include "gc.h"
#include "libft.h"
#include "utils.h"
#include <stdlib.h>

char	*handle_quoted_arg(char *arg, t_gc *gc)
{
	char	*trimmed;
	int		len;

	if ((arg[0] == '"' || arg[0] == '\'') && ft_strlen(arg) >= 2)
	{
		len = ft_strlen(arg);
		trimmed = gc_substr(gc, arg, 1, len - 2);
		if (!trimmed)
			return (NULL);
	}
	else
	{
		trimmed = gc_strdup(gc, arg);
		if (!trimmed)
			return (NULL);
	}
	return (trimmed);
}

int	validate_numeric_string(char *final_trimmed)
{
	int	i;

	i = 0;
	if (final_trimmed[i] == '-' || final_trimmed[i] == '+')
		i++;
	if (!final_trimmed[i])
		return (0);
	while (final_trimmed[i])
	{
		if (!ft_isdigit(final_trimmed[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	compare_with_limit(const char *str, int is_negative)
{
	char	*max_long;
	char	*min_long;
	int		i;

	max_long = "9223372036854775807";
	min_long = "9223372036854775808";
	i = 0;
	while (i < 19)
	{
		if (is_negative && str[i] > min_long[i])
			return (1);
		if (!is_negative && str[i] > max_long[i])
			return (1);
		if (is_negative && str[i] < min_long[i])
			return (0);
		if (!is_negative && str[i] < max_long[i])
			return (0);
		i++;
	}
	return (0);
}

static int	is_overflow(const char *str)
{
	int		len;
	int		is_negative;

	if (!str || !*str)
		return (1);
	if (*str == '-')
		is_negative = 1;
	else
		is_negative = 0;
	if (*str == '-' || *str == '+')
		str++;
	len = ft_strlen(str);
	if (len > 19)
		return (1);
	if (len < 19)
		return (0);
	return (compare_with_limit(str, is_negative));
}

int	check_exit_arg(char *arg, int *exit_code, t_gc *gc)
{
	long long	num;
	char		*trimmed;
	char		*final_trimmed;

	if (!arg)
		return (1);
	trimmed = handle_quoted_arg(arg, gc);
	if (!trimmed)
		return (0);
	final_trimmed = gc_strtrim(gc, trimmed, " \t\n\r\f\v");
	if (!final_trimmed)
		return (0);
	if (!validate_numeric_string(final_trimmed))
		return (0);
	if (is_overflow(final_trimmed))
		return (0);
	num = ft_atoll(final_trimmed);
	if (num > 255 || num < -255)
		*exit_code = normalize_exit_code((int)num);
	else
		*exit_code = (int)num;
	return (1);
}
