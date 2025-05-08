/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 04:58:08 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:10:06 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_get_env(t_data *d, char *key)
{
	t_env_data	*data;
	t_list		*tmp;

	tmp = d->env;
	while (tmp)
	{
		data = (t_env_data *)tmp->data;
		if (!ft_strncmp(data->key, key, ft_strlen(key) + 1))
			return (data->value);
		tmp = tmp->next;
	}
	return (NULL);
}

static char	*ft_create_env_string(t_env_data *env, bool all)
{
	char	*str;
	char	*tmp;

	if (all && !*(env->value) && !env->exported)
		str = ft_strdup(env->key);
	else
		str = ft_strjoin(env->key, "=");
	if (!str)
		return (ft_put_error(ERROR_MEM, NULL), NULL);
	if (all)
	{
		if (!*(env->value) && !env->exported)
			return (str);
		tmp = ft_straddchr(env->value, '"');
		if (!tmp)
			return (free(str), ft_put_error(ERROR_MEM, NULL), NULL);
		str = ft_strjoin_free(str, tmp, 3);
	}
	else
		str = ft_strjoin_free(str, env->value, 1);
	if (!str)
		return (ft_put_error(ERROR_MEM, NULL), NULL);
	return (str);
}

static size_t	ft_env_map_size(t_list *env, bool all)
{
	t_env_data	*data;
	t_list		*tmp;
	size_t		size;

	size = 0;
	tmp = env;
	while (tmp)
	{
		data = (t_env_data *)tmp->data;
		if (all || data->exported)
			size++;
		tmp = tmp->next;
	}
	return (size);
}

char	**ft_get_env_map(t_data *d, bool all)
{
	t_env_data	*data;
	t_list		*tmp;
	char		**map;
	size_t		i;

	map = (char **)malloc(sizeof(char *) * (ft_env_map_size(d->env, all) + 1));
	if (!map)
		return (ft_put_error(ERROR_MEM, NULL), NULL);
	i = 0;
	tmp = d->env;
	while (tmp)
	{
		data = (t_env_data *)tmp->data;
		if (all || data->exported)
		{
			map[i] = ft_create_env_string(data, all);
			if (!map[i])
				return (ft_mfree((void **)map, i), NULL);
			i++;
		}
		tmp = tmp->next;
	}
	map[i] = NULL;
	return (map);
}
