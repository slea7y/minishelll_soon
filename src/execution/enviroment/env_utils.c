/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: majkijew <majkijew@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:16:36 by majkijew          #+#    #+#             */
/*   Updated: 2025/10/03 22:59:47 by majkijew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../Includes/executor.h"
#include "../../../Includes/minishell.h"
#include "../../../libft/libft.h"

char	**free_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		free(env[i++]);
	free(env);
	return (NULL);
}

static int	env_to_array_loop(t_env_node *current, int i, char **envp)
{
	while (current)
	{
		if (current->value)
		{
			envp[i] = malloc(
					ft_strlen(current->key) + ft_strlen(current->value) + 2);
			if (!envp[i])
				return (-1);
			ft_strlcpy(envp[i], current->key, ft_strlen(current->key) + 1);
			ft_strlcat(envp[i], "=", ft_strlen(current->key) + 2);
			ft_strlcat(envp[i], current->value,
				ft_strlen(current->key) + ft_strlen(current->value) + 2);
			i++;
		}
		current = current->next;
	}
	return (i);
}

char	**env_to_array(t_env_list *env_list)
{
	char		**envp;
	int			i;

	if (!env_list || !env_list->head)
	{
		envp = malloc(sizeof(char *));
		if (!envp)
			return (NULL);
		envp[0] = NULL;
		return (envp);
	}
	envp = malloc(sizeof(char *) * (env_list->size + 1));
	if (!envp)
		return (NULL);
	i = env_to_array_loop(env_list->head, 0, envp);
	if (i == -1)
		return (free_env(envp));
	envp[i] = NULL;
	return (envp);
}
