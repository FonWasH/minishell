/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_subsyntax.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 20:34:57 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:09:40 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	ft_check_open_parenthesis(char *line, size_t current_pos)
{
	int	i;

	i = (int)current_pos;
	i--;
	while (i >= 0 && ft_isspace(line[i]))
		i--;
	if (i >= 0 && !ft_isctrlop(line[i]) && line[i] != '(')
		return (true);
	return (false);
}

static bool	ft_check_close_parenthesis(char *line, size_t current_pos)
{
	size_t	i;

	i = current_pos;
	i++;
	while (line[i] && ft_isspace(line[i]))
		i++;
	if (line[i] && !ft_isctrlop(line[i]) && line[i] != ')')
		return (true);
	return (false);
}

bool	ft_check_subline_syntax(char *line)
{
	size_t	i;

	i = 0;
	while (line[i])
	{
		ft_skip_quotes(line, &i);
		if (line[i] == '(' && i > 0
			&& ft_check_open_parenthesis(line, i))
			return (true);
		if (line[i] == ')' && line[i + 1]
			&& ft_check_close_parenthesis(line, i))
			return (true);
		i++;
	}
	return (false);
}
