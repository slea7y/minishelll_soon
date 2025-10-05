/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdietz-r <tdietz-r@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 03:56:15 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/05 14:03:13 by tdietz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

/// Ctrl-C
/// @brief 	rl_on_new_line() - reset readlines internal state
// rl_replace_line("", 0); -- clear current line buffer
// rl_redisplay();-- redisplay prompt
///
/// @param sig
static void	sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/// Ctrl-\ used to quit and dump core
/// i think its more usefull in child than parent idk i just dont use it
// normally
/// @param sig
static void	sigquit_handler(int sig)
{
	(void)sig;
	write(1, "Quit: 3\n", 8);
}

/// @brief used in heredocs only no redisplay no prompt just abort heredoc
/// @param sig
static void	handle_heredoc_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit(130);
}

void	start_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
}

void	setup_heredoc_and_child_sig(int is_heredoc)
{
	if (is_heredoc)
	{
		signal(SIGINT, handle_heredoc_sigint);
		signal(SIGQUIT, SIG_IGN);
	}
	else
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
}

// all of that and u may ask wheere is the Ctrl-D??
//!! let me take u to the main....