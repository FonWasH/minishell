/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 04:38:30 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:10:01 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char	**environ;

bool	ft_init_env(t_data *d)
{
	t_env_data	*new;
	char		*sep;
	size_t		i;

	if (d->env)
		ft_clear_env(&d->env);
	i = 0;
	while (environ[i])
	{
		new = (t_env_data *)malloc(sizeof(t_env_data));
		if (!new)
			return (ft_put_error(ERROR_MEM, NULL), false);
		sep = ft_strchr(environ[i], '=');
		new->key = ft_strldup(environ[i], sep - environ[i]);
		if (!new->key)
			return (ft_put_error(ERROR_MEM, NULL), free(new), false);
		new->value = ft_strdup(sep + 1);
		if (!new->value)
			return (ft_put_error(ERROR_MEM, NULL),
				free(new->key), free(new), false);
		new->exported = true;
		ft_lstadd_back(&d->env, ft_lstnew(new));
		i++;
	}
	return (ft_cd(d, NULL), true);
}

void	ft_clear_env(t_list **env)
{
	t_env_data	*data;
	t_list		*tmp;

	tmp = *env;
	while (tmp)
	{
		data = (t_env_data *)tmp->data;
		free(data->key);
		free(data->value);
		tmp = tmp->next;
	}
	ft_lstclear(env, free);
}
