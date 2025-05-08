/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 16:33:26 by cbaillet          #+#    #+#             */
/*   Updated: 2025/05/07 15:15:00 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_strdestar(char *str)
{
	size_t	i;
	size_t	end;

	i = 0;
	while (str[i])
	{
		ft_skip_quotes(str, &i);
		if (str[i] == '*')
		{
			end = i + 1;
			while (str[end] && str[end] == '*')
				end++;
			ft_memmove(str + i + 1, str + end, ft_strlen(str + end) + 1);
		}
		i++;
	}
	return (str);
}

static char	*ft_getpattern(char *str)
{
	char	*start;
	size_t	i;
	char	*end;

	i = 0;
	str = ft_strdestar(str);
	end = NULL;
	start = NULL;
	while (str[i])
	{
		ft_skip_quotes(str, &i);
		if (str[i] == '*')
		{
			start = str + i;
			end = str + i;
			while (start > str && !ft_isspace(*start) && !ft_isquote(*start))
				start--;
			while (*end && !ft_isspace(*end) && !ft_isquote(*end))
				end++;
			break ;
		}
		i++;
	}
	return (ft_substr(start, (ft_isspace(*start) || ft_isquote(*start)), end
			- start));
}

char	*ft_str_wildcard(char *str)
{
	char	*pattern;

	if (!ft_iswildcard(str))
		return (str);
	pattern = ft_getpattern(str);
	if (!pattern)
		return (ft_put_error(ERROR_MEM, NULL), NULL);
	return (ft_process_wildcard(str, pattern));
}
