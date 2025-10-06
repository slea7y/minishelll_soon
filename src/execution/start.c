/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:34:07 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/06 02:05:39 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/executor.h"

int	is_bulidin(char *cmd)
{
	if (!cmd)
		return (0);
	else
		return (ft_strcmp(cmd, "exit") == 0 || ft_strcmp(cmd, "echo") == 0
			|| ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "export") == 0
			|| ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "unset") == 0
			|| ft_strcmp(cmd, "pwd") == 0);
}

int	execute_bulidins(t_cmd_node *cmd, t_shell_ctx *ctx)
{
	int	exit_code;

	if (!cmd || !cmd->cmd[0])
		return (1);
	if (ft_strcmp(cmd->cmd[0], "echo") == 0)
		exit_code = ft_echo(cmd, ctx);
	else if (ft_strcmp(cmd->cmd[0], "env") == 0)
		exit_code = ft_env(cmd, ctx);
	else if (ft_strcmp(cmd->cmd[0], "export") == 0)
		exit_code = ft_export(cmd, ctx);
	else if (ft_strcmp(cmd->cmd[0], "cd") == 0)
		exit_code = ft_cd(cmd, ctx);
	else if (ft_strcmp(cmd->cmd[0], "unset") == 0)
		exit_code = ft_unset(cmd, ctx);
	else if (ft_strcmp(cmd->cmd[0], "pwd") == 0)
		exit_code = ft_pwd(cmd, ctx);
	else if (ft_strcmp(cmd->cmd[0], "exit") == 0)
		exit_code = ft_exit(cmd, ctx);
	else
		exit_code = 0;
	return (exit_code);
}

int	detect_pipes(t_cmd_list *cmds)
{
	t_cmd_node	*current;

	current = cmds->head;
	if (!cmds || !cmds->head)
		return (0);
	while (current)
	{
		if (current->next)
			return (1);
		current = current->next;
	}
	return (0);
}

static int	execute_single_command(t_cmd_node *current, t_shell_ctx *ctx)
{
	int	exit_code;

	if (process_heredoc(current, ctx) != 0)
		return (1);
	if (current->cmd && current->cmd[0] && is_bulidin(current->cmd[0]))
	{
		if (handle_redirections(current) != 0)
			exit_code = 1;
		else
			exit_code = execute_bulidins(current, ctx);
	}
	else if (current->cmd && current->cmd[0])
		exit_code = execute_single_external(current, ctx);
	else
	{
		if (current->files && current->files->head)
			exit_code = execute_single_external(current, ctx);
		else
			exit_code = 0;
	}
	return (exit_code);
}

int	start_execution(t_cmd_list *cmds, t_shell_ctx *ctx)
{
	t_cmd_node	*current;

	if (!cmds || !cmds->head)
		return (0);
	current = cmds->head;
	if (!detect_pipes(cmds))
		return (execute_single_command(current, ctx));
	else
		return (execute_pipes(cmds, ctx));
}
