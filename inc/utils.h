/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:34:00 by kalkan            #+#    #+#             */
/*   Updated: 2025/08/30 12:30:53 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "gc.h"
# include "lexer.h"

typedef struct s_env_node	t_env_node;
typedef struct s_context	t_context;

int			ft_strcmp(const char *s1, const char *s2);
char		*ft_strcpy(char *dst, const char *src);
char		*ft_strcat(char *dst, const char *src);
int			is_valid_char(char c);
char		*extract_var_name(t_gc *gc, const char *str);
char		*get_env_value(t_env_node *head, const char *key);
void		free_string_array(char **array);
int			argument_count(char **argv);
void		clean_exit(t_context *ctx);
void		print_error_message(const char *cmd,
				const char *arg, const char *msg);
void		print_warning_message_with_line(const char *delimiter,
				int line_number);
char		**split_and_count(t_gc *gc, char *expanded,
				int *word_count);
int			split_env_var(t_context *ctx, char *arg,
				char **key_out, char **value_out);
int			validate_redirect_next(t_token *current);
void		clean_empty_quote_tokens(t_token **token_list);
int			is_whitespace_only(const char *str);
int			is_directory(const char *path);
long long	ft_atoll(const char *str);

#endif
