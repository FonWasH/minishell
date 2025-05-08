/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 05:46:04 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:07:33 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_setpath(t_data *d, char *old)
{
	char	*new;

	new = getcwd(NULL, 0);
	if (!new)
		return (ft_put_error(strerror(errno), NULL), free(old), EXIT_FAILURE);
	if (!ft_set_env(d, PWD, new, true) || !ft_set_env(d, OLDPWD, old, true))
		return (free(old), free(new), EXIT_FAILURE);
	return (free(old), free(new), EXIT_SUCCESS);
}

int	ft_cd(t_data *d, char **args)
{
	char	*path;
	char	*old;

	if (args && ft_mstrlen(args) > 1)
		return (ft_put_error(ERROR_ARG, NULL), EXIT_FAILURE);
	if (!args || !args[0] || !*args[0])
		path = ft_get_env(d, HOME);
	else
		path = args[0];
	if (!path)
		return (EXIT_FAILURE);
	old = getcwd(NULL, 0);
	if (!old)
		return (ft_put_error(strerror(errno), NULL), EXIT_FAILURE);
	if (chdir(path) == ERROR)
	{
		ft_put_error(strerror(errno), path);
		return (free(old), EXIT_FAILURE);
	}
	return (ft_setpath(d, old));
}
