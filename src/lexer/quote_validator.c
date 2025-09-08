/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_validator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:29:52 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/22 09:59:18 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static void	update_local_quote_state(char c, t_quote_state *state)
{
	if (state->in_single_quote)
	{
		if (c == '\'')
			state->in_single_quote = 0;
	}
	else if (state->in_double_quote)
	{
		if (c == '"')
			state->in_double_quote = 0;
	}
	else
	{
		if (c == '\'')
			state->in_single_quote = 1;
		else if (c == '"')
			state->in_double_quote = 1;
	}
}

static char	get_missing_quote_type(const t_quote_state *state)
{
	if (state->in_single_quote)
		return ('\'');
	if (state->in_double_quote)
		return ('"');
	return (0);
}

int	validate_quotes(const char *line, char *missing_quote_type)
{
	t_quote_state	state;
	int				i;

	i = 0;
	state.in_single_quote = 0;
	state.in_double_quote = 0;
	*missing_quote_type = 0;
	while (line[i])
	{
		update_local_quote_state(line[i], &state);
		i++;
	}
	*missing_quote_type = get_missing_quote_type(&state);
	return (state.in_single_quote == 0 && state.in_double_quote == 0);
}
