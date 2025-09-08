/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:58:30 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/14 15:58:30 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "env.h"
#include "libft.h"
#include "utils.h"

static void	print_export_list(t_env_node *env_list)
{
	t_env_node	*curr;

	curr = env_list;
	while (curr != NULL)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(curr->key, 1);
		if (curr->value
			&& ft_strcmp(curr->value, "__EXPORTED_WITHOUT_VALUE__") != 0)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(curr->value, 1);
			ft_putstr_fd("\"\n", 1);
		}
		else
			ft_putchar_fd('\n', 1);
		curr = curr->next;
	}
}

static int	validate_and_parse_arg(t_context *ctx, char *arg, char **key_out,
		char **value_out)
{
	char	*key;
	char	*value;

	if (!split_env_var(ctx, arg, &key, &value))
		return (0);
	if (!is_valid_name(key))
	{
		ft_putstr_fd("babyshell🎀: export: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": not a valid identifier\n", 2);
		gc_free(&ctx->gc, key);
		if (value)
			gc_free(&ctx->gc, value);
		return (0);
	}
	*key_out = key;
	*value_out = value;
	return (1);
}

static void	assign_export_variable(t_context *ctx, char *key, char *value)
{
	t_env_node	*existing;

	if (value)
		env_assign(ctx, key, value);
	else
	{
		existing = env_find(ctx, key);
		if (!existing)
			env_assign(ctx, key, "__EXPORTED_WITHOUT_VALUE__");
	}
}

static int	handle_export_arg(t_context *ctx, char *arg)
{
	char	*key;
	char	*value;

	if (!validate_and_parse_arg(ctx, arg, &key, &value))
		return (0);
	assign_export_variable(ctx, key, value);
	gc_free(&ctx->gc, key);
	if (value)
		gc_free(&ctx->gc, value);
	return (1);
}

int	built_export(t_context *ctx, char **args)
{
	int	i;
	int	exit_code;

	i = 1;
	exit_code = EXIT_SUCCESS;
	if (!args[1])
	{
		print_export_list(ctx->env);
		return (EXIT_SUCCESS);
	}
	while (args[i])
	{
		if (!handle_export_arg(ctx, args[i]))
			exit_code = EXIT_FAILURE;
		i++;
	}
	return (exit_code);
}
