/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalkan <kalkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 15:20:40 by kalkan            #+#    #+#             */
/*   Updated: 2025/09/01 11:59:38 by kalkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "executor.h"
#include "libft.h"
#include "shell_stages.h"
#include "signal_handler.h"
#include "utils.h"

static int	handle_input_line(t_context *ctx, t_shell *shell, char *line)
{
	if (g_signal == SIGINT)
	{
		shell->last_exit_status = 130;
		g_signal = 0;
		if (line[0] == '\0')
		{
			gc_free(&ctx->gc, line);
			return (1);
		}
	}
	if (line[0] == '\0')
	{
		gc_free(&ctx->gc, line);
		return (1);
	}
	process_shell_line(line, ctx, shell);
	return (1);
}

static void	shell_loop(t_context *ctx, t_shell *shell)
{
	char	*line;

	while (1)
	{
		if (shell && shell->should_exit)
			break ;
		set_signal_handler(PROMT_SIG);
		line = gc_readline(&ctx->gc, "babyshell🎀$");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		handle_input_line(ctx, shell, line);
	}
	clean_exit(ctx);
}

int	main(int argc, char **argv, char **envp)
{
	t_context	ctx;
	t_shell		shell;

	if (argc != 1)
	{
		print_error_message(NULL, argv[1], "No such file or directory");
		return (127);
	}
	ft_memset(&ctx, 0, sizeof(t_context));
	gc_init(&ctx.gc);
	ctx.heredoc_list = NULL;
	ctx.current_heredoc_content = NULL;
	env_initialize(&ctx, envp);
	set_signal_handler(PROMT_SIG);
	shell.env_list = ctx.env;
	shell.last_exit_status = EXIT_SUCCESS;
	shell.should_exit = 0;
	shell.last_arg = NULL;
	ctx.shell = &shell;
	shell_loop(&ctx, &shell);
	restore_signal_handlers();
	clean_exit(&ctx);
	return (shell.last_exit_status);
}
