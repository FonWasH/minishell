/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chr_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 20:14:29 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:09:57 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_isop(char c)
{
	return (c == '|' || c == '&' || c == '<' || c == '>');
}

bool	ft_isfileop(char c)
{
	return (c == '<' || c == '>');
}

bool	ft_isctrlop(char c)
{
	return (c == '|' || c == '&');
}

bool	ft_ismeta(char c)
{
	return (c == '\\' || c == ';');
}

bool	ft_isquote(char c)
{
	return (c == '\'' || c == '\"');
}
