/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 05:32:14 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:07:41 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_data *d)
{
	char	**env;
	size_t	i;

	env = ft_get_env_map(d, false);
	if (!env)
		return (EXIT_FAILURE);
	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
	return (ft_mfree((void **)env, ft_mstrlen(env)), EXIT_SUCCESS);
}
