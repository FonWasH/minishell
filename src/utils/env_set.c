/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 05:09:43 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:10:11 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	ft_add_env(t_data *d, char *key, char *value, bool exported)
{
	t_env_data	*new;

	new = (t_env_data *)malloc(sizeof(t_env_data));
	if (!new)
		return (ft_put_error(ERROR_MEM, NULL), false);
	new->key = ft_strdup(key);
	if (!new->key)
		return (ft_put_error(ERROR_MEM, NULL), free(new), false);
	new->value = ft_strdup(value);
	if (!new->value)
		return (ft_put_error(ERROR_MEM, NULL),
			free(new->key), free(new), false);
	new->exported = exported;
	return (ft_lstadd_back(&d->env, ft_lstnew(new)), true);
}

bool	ft_set_env(t_data *d, char *key, char *value, bool exported)
{
	t_env_data	*data;
	t_list		*tmp;

	tmp = d->env;
	while (tmp)
	{
		data = (t_env_data *)tmp->data;
		if (!ft_strncmp(data->key, key, ft_strlen(key) + 1))
		{
			free(data->value);
			data->value = ft_strdup(value);
			if (!data->value)
				return (ft_put_error(ERROR_MEM, NULL), ft_init_env(d), false);
			data->exported = exported;
			return (true);
		}
		tmp = tmp->next;
	}
	return (ft_add_env(d, key, value, exported));
}

bool	ft_unset_env(t_data *d, char *key)
{
	t_env_data	*data;
	t_list		*tmp;

	tmp = d->env;
	while (tmp)
	{
		data = (t_env_data *)tmp->data;
		if (!ft_strncmp(data->key, key, ft_strlen(key) + 1))
			return (free(data->key), free(data->value),
				ft_lstdelnode(&d->env, tmp), true);
		tmp = tmp->next;
	}
	return (true);
}
