/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_checkpath.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 22:30:05 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:14:23 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_ispath_dir(char *path)
{
	t_stat	statbuf;

	if (stat(path, &statbuf))
		return (false);
	return (S_ISDIR(statbuf.st_mode));
}

bool	ft_ispath_file(char *path)
{
	t_stat	statbuf;

	if (stat(path, &statbuf))
		return (false);
	return (S_ISREG(statbuf.st_mode));
}

bool	ft_ispath_exec(char *path)
{
	t_stat	statbuf;

	if (stat(path, &statbuf))
		return (false);
	return (statbuf.st_mode & S_IXUSR);
}

bool	ft_ispath(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '/')
			return (true);
		i++;
	}
	return (false);
}
