/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_skipquotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 20:12:32 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:14:40 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_skip_quote(char *str, size_t *i)
{
	if (str[*i] == '\'')
	{
		*i += 1;
		while (str[*i] && str[*i] != '\'')
			*i += 1;
	}
}

void	ft_skip_dquote(char *str, size_t *i)
{
	if (str[*i] == '\"')
	{
		*i += 1;
		while (str[*i] && str[*i] != '\"')
			*i += 1;
	}
}

void	ft_skip_quotes(char *str, size_t *i)
{
	ft_skip_quote(str, i);
	ft_skip_dquote(str, i);
}
