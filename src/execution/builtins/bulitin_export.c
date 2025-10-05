/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bulitin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 22:00:01 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/05 05:04:23 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/executor.h"
#include "../../../Includes/minishell.h"
#include "../../../libft/libft.h"

int	check_if_var_exist(char *key, t_env_list *env)
{
	t_env_node	*current;

	if (!key || !env)
		return (0);
	current = env->head;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (1);
		current = current->next;
	}
	return (0);
}

int	is_valid_identifier(char *args)
{
	int	i;

	if (!args || !args[0])
		return (0);
	if (!ft_isalpha(args[0]) && args[0] != '_')
		return (0);
	i = 0;
	while (args[i] != '=' && args[i])
	{
		if (!ft_isalnum(args[i]) && args[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	print_export_format(t_env_list *env)
{
	t_env_node	*current;

	if (!env)
		return ;
	current = env->head;
	while (current)
	{
		if (current->value)
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(current->key, 1);
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(current->value, 1);
			ft_putstr_fd("\"\n", 1);
		}
		current = current->next;
	}
}

static int	process_export_args(t_cmd_node *cmd, t_shell_ctx *ctx)
{
	int	i;
	int	exit_code;

	i = 1;
	exit_code = 0;
	while (cmd->cmd[i])
	{
		if (!is_valid_identifier(cmd->cmd[i]))
		{
			ft_putstr_fd(cmd->cmd[i], 2);
			ft_putstr_fd(": not a valid identifier\n", 2);
			exit_code = 1;
		}
		else
			add_change_export(cmd->cmd[i], ctx->env);
		i++;
	}
	return (exit_code);
}

int	ft_export(t_cmd_node *cmd, t_shell_ctx *ctx)
{
	if (!ctx || !ctx->env)
		return (1);
	if (!cmd->cmd[1])
	{
		sort_env(ctx->env);
		print_export_format(ctx->env);
		return (0);
	}
	return (process_export_args(cmd, ctx));
}
