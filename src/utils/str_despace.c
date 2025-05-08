/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_despace.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 22:05:04 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:14:30 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_str_despace(char *str)
{
	size_t	i;
	size_t	end;

	i = 0;
	while (str[i])
	{
		ft_skip_quotes(str, &i);
		if (ft_isspace(str[i]))
		{
			end = i + 1;
			while (str[end] && ft_isspace(str[end]))
				end++;
			ft_memmove(str + i + (1 * (str[end] && (i > 0))),
				str + end, ft_strlen(str + end) + 1);
		}
		i++;
	}
	return (str);
}
