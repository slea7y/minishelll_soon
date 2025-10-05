/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 17:11:28 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/05 03:43:58 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/executor.h"
#include "../../../Includes/minishell.h"
#include "../../../libft/libft.h"

char	*bulid_absolute_path(char *relative_path)
{
	char	*curr;
	char	*temp;
	char	*full_path;

	if (relative_path[0] == '/')
		return (ft_strdup(relative_path));
	curr = getcwd(NULL, 0);
	if (!curr)
		return (NULL);
	temp = ft_strjoin(curr, "/");
	free(curr);
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, relative_path);
	free(temp);
	return (full_path);
}

int	go_dest_with_path(char *path, char *prev_pwd, t_env_list *env)
{
	char	*abdsolute_path;

	if (path[0] == '/' || ft_strcmp(path, "..") == 0
		|| ft_strcmp(path, ".") == 0)
		abdsolute_path = ft_strdup(path);
	else
		abdsolute_path = bulid_absolute_path(path);
	if (!abdsolute_path)
		return (free(prev_pwd), 1);
	if (chdir(abdsolute_path) != 0)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free(abdsolute_path);
		free(prev_pwd);
		return (1);
	}
	change_var_value("OLDPWD", prev_pwd, env);
	change_var_value("PWD", abdsolute_path, env);
	free(abdsolute_path);
	free(prev_pwd);
	return (0);
}
