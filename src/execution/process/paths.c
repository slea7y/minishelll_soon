/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 22:31:00 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/05 03:23:17 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/minishell.h"
#include "../../../Includes/executor.h"
#include "../../../Includes/parser.h"

char	*build_cmd_paths(char *dir, char *cmd)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	return (full_path);
}

void	failed_paths(char **paths, char *cmd)
{
	int	i;

	if (!paths)
	{
		i = 0;
		while (paths[i])
			free(paths[i++]);
		free(paths);
	}
	if (cmd && ft_strcmp(cmd, "-1") != 0)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	exit(127);
}

char	**find_full_path(t_env_list *env_list)
{
	char	*path_val;
	char	**paths;

	path_val = get_env_value(env_list, "PATH");
	if (!path_val || path_val[0] == '\0')
	{
		if (path_val)
			free(path_val);
		return (NULL);
	}
	paths = ft_split(path_val, ':');
	free(path_val);
	if (!paths)
		return (NULL);
	return (paths);
}
