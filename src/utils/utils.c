/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:15:20 by kalkan            #+#    #+#             */
/*   Updated: 2025/09/01 11:52:36 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "executor.h"
#include "libft.h"
#include "utils.h"
#include <unistd.h>
#include <readline/readline.h>

void	clean_exit(t_context *ctx)
{
	int	fd;

	if (ctx)
	{
		gc_clear_all(&ctx->gc);
		ctx->env = NULL;
	}
	rl_clear_history();
	fd = 3;
	while (fd < 1024)
	{
		close(fd);
		fd++;
	}
}

char	**split_and_count(t_gc *gc, char *expanded, int *word_count)
{
	char	**words;

	words = gc_split(gc, expanded, ' ');
	if (!words)
		return (NULL);
	*word_count = 0;
	while (words[*word_count])
		(*word_count)++;
	return (words);
}

int	split_env_var(t_context *ctx, char *arg, char **key_out, char **value_out)
{
	char	*equal_sign;
	char	*key;
	char	*value;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		key = gc_substr(&ctx->gc, arg, 0, equal_sign - arg);
		if (!key)
			return (0);
		value = gc_strdup(&ctx->gc, equal_sign + 1);
		if (!value)
			return (gc_free(&ctx->gc, key), 0);
	}
	else
	{
		key = gc_strdup(&ctx->gc, arg);
		if (!key)
			return (0);
		value = NULL;
	}
	*key_out = key;
	*value_out = value;
	return (1);
}

int	validate_redirect_next(t_token *current)
{
	if (current->next->type >= TOKEN_REDIRECT_OUT
		&& current->next->type <= TOKEN_HEREDOC)
	{
		if (current->next->type == TOKEN_REDIRECT_OUT)
			print_error_message(NULL, NULL,
				"syntax error near unexpected token `>'");
		else if (current->next->type == TOKEN_REDIRECT_IN)
			print_error_message(NULL, NULL,
				"syntax error near unexpected token `<'");
		else if (current->next->type == TOKEN_APPEND)
			print_error_message(NULL, NULL,
				"syntax error near unexpected token `>>'");
		else if (current->next->type == TOKEN_HEREDOC)
			print_error_message(NULL, NULL,
				"syntax error near unexpected token `<<'");
		return (0);
	}
	if (current->next->type == TOKEN_PIPE)
	{
		print_error_message(NULL, NULL,
			"syntax error near unexpected token `|'");
		return (0);
	}
	return (1);
}

int	is_whitespace_only(const char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
			return (0);
		i++;
	}
	return (i > 0);
}
