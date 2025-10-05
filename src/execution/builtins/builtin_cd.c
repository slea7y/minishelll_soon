/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 17:16:52 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/05 03:29:36 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/executor.h"
#include "../../../Includes/parser.h"
#include "../../../Includes/minishell.h"

char	*expand_tilde(char *path, t_env_list *env)
{
	char	*home;
	char	*result;

	if (!path || path[0] != '~')
		return (ft_strdup(path));
	if (path[1] == '\0' || path[1] == '/')
	{
		home = get_env_value(env, "HOME");
		if (!home || home[0] == '\0')
		{
			if (home)
				free(home);
			return (ft_strdup(path));
		}
		if (path[1] == '\0')
			result = ft_strdup(home);
		else
			result = ft_strjoin(home, &path[1]);
		free(home);
		return (result);
	}
	return (ft_strdup(path));
}

int	handle_cd_oldpwd(char *prev_pwd, t_env_list *env)
{
	char	*old_pwd;

	old_pwd = get_env_value(env, "OLDPWD");
	if (!old_pwd || old_pwd[0] == '\0')
	{
		ft_putstr_fd("cd: OLDPWD not set\n", 2);
		free(prev_pwd);
		if (old_pwd)
			free(old_pwd);
		return (1);
	}
	ft_putstr_fd(old_pwd, 2);
	ft_putstr_fd("\n", 2);
	if (chdir(old_pwd) != 0)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(old_pwd, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (free(old_pwd), free(prev_pwd), 1);
	}
	change_var_value("OLDPWD", prev_pwd, env);
	change_var_value("PWD", old_pwd, env);
	return (free(old_pwd), free(prev_pwd), 0);
}

//maybe i will need to change the errror message 
int	go_home(char *prev_pwd, t_env_list *env)
{
	char	*home;

	home = get_env_value(env, "HOME");
	if (!home)
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		free(prev_pwd);
		return (1);
	}
	if (chdir(home) != 0)
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		free(home);
		free(prev_pwd);
		return (1);
	}
	change_var_value("OLDPWD", prev_pwd, env);
	change_var_value("PWD", home, env);
	free(home);
	free(prev_pwd);
	return (0);
}

int	ft_cd(t_cmd_node *cmd, t_shell_ctx *ctx)
{
	char	*prev_pwd;
	char	*expand_path;
	int		ret;

	prev_pwd = getcwd(NULL, 0);
	if (!prev_pwd)
	{
		perror("getcwd");
		return (1);
	}
	if (!cmd->cmd[1] || (ft_strcmp(cmd->cmd[1], "~") == 0))
		return (go_home(prev_pwd, ctx->env));
	if (cmd->cmd[2] != NULL)
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (free(prev_pwd), 1);
	}
	if (ft_strcmp(cmd->cmd[1], "-") == 0)
		return (handle_cd_oldpwd(prev_pwd, ctx->env));
	expand_path = expand_tilde(cmd->cmd[1], ctx->env);
	if (!expand_path)
		return (free(prev_pwd), 1);
	ret = go_dest_with_path(expand_path, prev_pwd, ctx->env);
	return (free(expand_path), ret);
}
