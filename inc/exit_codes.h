/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_codes.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:33:40 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/14 15:33:41 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXIT_CODES_H
# define EXIT_CODES_H

typedef struct s_shell	t_shell;

# define EXIT_SUCCESS 0
# define EXIT_GENERAL_ERROR 1
# define EXIT_SYNTAX_ERROR 2
# define EXIT_CANNOT_EXECUTE 126
# define EXIT_COMMAND_NOT_FOUND 127

int						normalize_exit_code(int exit_code);
void					set_exit_status(t_shell *shell, int exit_code);
int						get_command_exit_code(char *cmd_path);
int						handle_process_exit_code(int status);

#endif
