/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 20:08:44 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/06 01:31:52 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/executor.h"
#include "../../../Includes/minishell.h"
#include "../../../libft/libft.h"

int	n_flag_echo(t_cmd_node *cmd, int *i)
{
	int	j;
	int	n_flag;

	n_flag = 0;
	while (cmd->cmd[*i] && cmd->cmd[*i][0] == '-')
	{
		j = 1;
		while (cmd->cmd[*i][j] == 'n')
			j++;
		if (cmd->cmd[*i][j] == '\0' && j > 1)
		{
			n_flag = 1;
			(*i)++;
		}	
		else
			break ;
	}
	return (n_flag);
}

int	ft_echo(t_cmd_node *cmd, t_shell_ctx *ctx)
{
	int	i;
	int	n_flag;

	(void)ctx;
	i = 1;
	n_flag = n_flag_echo(cmd, &i);
	while (cmd->cmd[i])
	{
		ft_putstr_fd(cmd->cmd[i], 1);
		i++;
		if (cmd->cmd[i])
			ft_putstr_fd(" ", 1);
	}
	if (!n_flag)
		ft_putstr_fd("\n", 1);
	return (0);
}

int	ft_pwd(t_cmd_node *cmd, t_shell_ctx *ctx)
{
	char	*pwd;
	int		exit_code;

	exit_code = 0;
	(void)cmd;
	(void)ctx;
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		ft_putstr_fd("pwd: error retrieving current directory\n", 2);
		return (1);
	}
	else
	{
		ft_putstr_fd(pwd, 1);
		ft_putstr_fd("\n", 1);
		free(pwd);
	}
	return (exit_code);
}

int	ft_exit(t_cmd_node *cmd, t_shell_ctx *ctx)
{
	int	exit_code;

	(void)ctx;
	exit_code = 0;
	if (!cmd->cmd[1])
		exit(exit_code);
	if (cmd->cmd[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		exit(1);
	}
	if (!is_numeric(cmd->cmd[1]))
	{
		ft_putstr_fd("exit: numeric argument required\n", 2);
		exit(2);
	}
	exit_code = ft_atoi(cmd->cmd[1]);
	exit_code = exit_code % 256;
	if (exit_code < 0)
		exit_code = 256 + exit_code;
	exit(exit_code);
}
