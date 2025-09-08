/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_parser_handler.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:54:05 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/14 14:54:06 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exit_codes.h"
#include "parser.h"

int	handle_parser_stage(t_token *token_list, t_ast_node **ast, t_gc *gc)
{
	*ast = parse(token_list, gc);
	if (!*ast)
		return (EXIT_GENERAL_ERROR);
	return (EXIT_SUCCESS);
}
