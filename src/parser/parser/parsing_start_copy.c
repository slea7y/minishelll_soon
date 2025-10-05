/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_start_copy.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 05:23:00 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/05 14:03:33 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"
#include "../../../Includes/parser.h"

// ok
/// @brief controll fction to add new nodes to cmd_struct
/// @param p
static void	handle_word_token(t_parser_ctx *p)
{
	if (!*p->current_cmd)
	{
		*p->current_cmd = create_cmd_node(*p->token);
		if (!*p->current_cmd)
		{
			p->cmd_list->syntax_error = 1;
			return ;
		}
		add_to_cmd_list(p->cmd_list, *p->current_cmd);
	}
	else
	{
		if (!add_argument(*p->current_cmd, (*p->token)->value))
			p->cmd_list->syntax_error = 1;
	}
	*p->token = (*p->token)->next;
}

// ok
/// @brief nulls command, so handle_word_token
// knows it need to create new cmd_node after |
/// @param p
static void	handle_pipe_token(t_parser_ctx *p)
{
	*p->current_cmd = NULL;
	*p->token = (*p->token)->next;
}

// ok
/// @brief comes into play when redirection before command token
// redirections always have to have a cmd node to bind to
static void	handle_redirection_token(t_parser_ctx *p)
{
	if (!*p->current_cmd)
	{
		*p->current_cmd = malloc(sizeof(t_cmd_node));
		if (!*p->current_cmd)
		{
			p->cmd_list->syntax_error = 1;
			return ;
		}
		(*p->current_cmd)->next = NULL;
		(*p->current_cmd)->cmd_type = CMD;
		(*p->current_cmd)->cmd = NULL;
		(*p->current_cmd)->files = NULL;
		add_to_cmd_list(p->cmd_list, *p->current_cmd);
	}
	add_redirection(*p->current_cmd, *p->token, (*p->token)->next,
		p->original_input);
	*p->token = (*p->token)->next->next;
}

// ok
/// @brief init cmd list
/// @param
/// @return
static t_cmd_list	*init_cmd_list(void)
{
	t_cmd_list	*cmd_list;

	cmd_list = malloc(sizeof(t_cmd_list));
	if (!cmd_list)
		return (NULL);
	cmd_list->head = NULL;
	cmd_list->tail = NULL;
	cmd_list->size = 0;
	cmd_list->syntax_error = 0;
	return (cmd_list);
}

// ok
/// @brief controll fction for parser
/// @param tokens
/// @param ctx
/// @param original_input
/// @return
t_cmd_list	*start_parser(t_token_list *tokens, t_shell_ctx *ctx,
		char *original_input)
{
	t_parser_ctx	p;
	t_cmd_node		*current_cmd;
	t_token			*token;

	(void)ctx;
	p.cmd_list = init_cmd_list();
	if (!p.cmd_list)
		return (NULL);
	current_cmd = NULL;
	token = tokens->head;
	p.current_cmd = &current_cmd;
	p.token = &token;
	p.original_input = original_input;
	while (token && !p.cmd_list->syntax_error)
	{
		if (token->token == TOKEN_WORD || token->token == TOKEN_BUILTIN)
			handle_word_token(&p);
		else if (token->token == TOKEN_PIPE)
			handle_pipe_token(&p);
		else if (token->token >= TOKEN_OUTFILE && token->token <= TOKEN_HEREDOC)
			handle_redirection_token(&p);
		else
			token = token->next;
	}
	return (p.cmd_list);
}
