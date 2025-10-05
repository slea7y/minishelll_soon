/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 22:02:12 by tdietz-r          #+#    #+#             */
/*   Updated: 2025/10/05 16:56:57 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Includes/executor.h"
#include "Includes/minishell.h"
#include "Includes/parser.h"

static char	*get_input(void)
{
	char	*input;

	if (isatty(fileno(stdin)))
		input = readline("minishell$ ");
	else
	{
		input = get_next_line(STDIN_FILENO);
		if (input && input[ft_strlen(input) - 1] == '\n')
			input[ft_strlen(input) - 1] = '\0';
	}
	return (input);
}

static int	handle_lexer_error(t_token_list *tokens, char *input,
		t_shell_ctx *ctx)
{
	ctx->last_exit_code = 2;
	free_token_list(tokens);
	free(input);
	return (0);
}

int	handle_parser_error(t_cmd_list *cmds, t_token_list *tokens, char *input,
		t_shell_ctx *ctx)
{
	ctx->last_exit_code = 2;
	if (cmds)
		free_cmd_list(cmds);
	if (tokens)
		free_token_list(tokens);
	if (input)
		free(input);
	return (1);
}

static int	process_input(char *input, t_shell_ctx *ctx)
{
	t_token_list	*tokens;
	t_cmd_list		*cmds;
	int				last_status;

	tokens = start_lexer(input);
	if (!tokens || tokens->found_error)
		return (handle_lexer_error(tokens, input, ctx));
	start_segmentation(tokens, ctx);
	cmds = start_parser(tokens, ctx, input);
	if (!cmds || cmds->syntax_error)
		return (handle_parser_error(cmds, tokens, input, ctx));
	last_status = start_execution(cmds, ctx);
	ctx->last_exit_code = last_status;
	if (cmds)
		free_cmd_list(cmds);
	if (tokens)
		free_token_list(tokens);
	if (input)
		free(input);
	return (1);
}

int	main_loop(char *input, t_shell_ctx *ctx)
{
	if (input[0] == '\0')
	{
		free(input);
		return (0);
	}
	if (isatty(fileno(stdin)))
		add_history(input);
	process_input(input, ctx);
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	char		*input;
	t_env_list	env_list;
	t_shell_ctx	ctx;

	(void)av;
	(void)ac;
	start_signals();
	init_list_env(&env_list, envp);
	ctx.env = &env_list;
	ctx.last_exit_code = 0;
	while (1)
	{
		input = get_input();
		if (!input)
		{
			if (isatty(fileno(stdin)))
				ft_putstr_fd("exit\n", 1);
			break ;
		}
		main_loop(input, &ctx);
	}
	// rl_clear_history();
	free_env_list(&env_list);
	free(input);
	return (ctx.last_exit_code);
}
