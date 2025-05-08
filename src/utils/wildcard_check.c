/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 15:18:54 by cbaillet          #+#    #+#             */
/*   Updated: 2025/05/07 15:14:47 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	ft_isnotpath(char *str, size_t pos)
{
	int	i;

	i = (int)pos;
	while (str[i] && !ft_isspace(str[i]) && !ft_isquote(str[i]))
	{
		if (str[i] == '/')
			return (false);
		i++;
	}
	i = (int)pos;
	while (i >= 0 && !ft_isspace(str[i]) && !ft_isquote(str[i]))
	{
		if (str[i] == '/')
			return (false);
		i--;
	}
	return (true);
}

bool	ft_iswildcard(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		ft_skip_quotes(str, &i);
		if (str[i] == '*')
			return (ft_isnotpath(str, i));
		i++;
	}
	return (false);
}

bool	ft_is_valid_file(struct dirent *dirent)
{
	return (ft_strncmp(dirent->d_name, ".", 2) && ft_strncmp(dirent->d_name,
			"..", 3));
}

bool	ft_match(char *pattern, char *str)
{
	if (!*pattern)
		return (!*str);
	if (*pattern == '*')
	{
		if (!*(pattern + 1))
			return (true);
		while (*str)
		{
			if (ft_match(pattern + 1, str))
				return (true);
			str++;
		}
		return (false);
	}
	if (*str && *pattern == *str)
		return (ft_match(pattern + 1, str + 1));
	return (false);
}
