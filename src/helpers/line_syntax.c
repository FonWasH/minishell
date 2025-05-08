/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_syntax.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 20:33:16 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:09:43 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_init_vars(bool *b, int *popen, int *pclose)
{
	b[QUOTE] = false;
	b[DQUOTE] = false;
	b[META] = false;
	b[CTRLOP] = false;
	*popen = 0;
	*pclose = 0;
}

static bool	ft_check_op_syntax(char *line)
{
	size_t	i;

	if (ft_isop(*line) && ft_isop(*(line + 1))
		&& ((*line != *(line + 1)) || ft_isop(*(line + 2))))
		return (true);
	if (ft_isop(*line) && !ft_isop(*(line + 1))
		&& *line == '&' && *(line - 1) != '&')
		return (true);
	if (ft_isctrlop(*line) && ft_isspace(*(line + 1)))
	{
		i = 1;
		while (ft_isspace(line[i]))
			i++;
		if (ft_isctrlop(line[i]))
			return (true);
	}
	if (ft_isfileop(*line) && ft_isspace(*(line + 1)))
	{
		i = 1;
		while (ft_isspace(line[i]))
			i++;
		if (ft_isop(line[i]))
			return (true);
	}
	return (false);
}

static void	ft_check_chr_syntax(bool b[4], char *line, int *popen, int *pclose)
{
	if (!b[DQUOTE] && *line == '\'')
		b[QUOTE] = !b[QUOTE];
	else if (!b[QUOTE] && *line == '\"')
		b[DQUOTE] = !b[DQUOTE];
	else if (!b[QUOTE] && !b[DQUOTE] && *line == '(')
		*popen += 1;
	else if (!b[QUOTE] && !b[DQUOTE] && *line == ')')
		*pclose += 1;
	else if (!b[QUOTE] && !b[DQUOTE] && !b[META] && ft_ismeta(*line))
		b[META] = true;
	else if (!b[QUOTE] && !b[DQUOTE] && !b[CTRLOP] && ft_check_op_syntax(line))
		b[CTRLOP] = true;
}

static bool	ft_ignore_line(char *line)
{
	if (ft_strlen(line) == 1)
	{
		if (line[0] == '.')
			return (ft_put_error(ERROR_FIL, NULL), true);
		if (line[0] == ':' || line[0] == '#' || line[0] == '!')
			return (true);
	}
	return (false);
}

bool	ft_check_line_syntax(char *line)
{
	bool	b[4];
	int		popen;
	int		pclose;

	line = ft_str_despace(line);
	if (!*line)
		return (true);
	if (ft_ignore_line(line))
		return (true);
	ft_init_vars(b, &popen, &pclose);
	b[CTRLOP] = (ft_isctrlop(*line)
			|| ft_isctrlop(*(line + ft_strlen(line) - 1))
			|| ft_isfileop(*(line + ft_strlen(line) - 1)));
	if (!b[CTRLOP])
		b[CTRLOP] = ft_check_subline_syntax(line);
	while (*line && !b[META] && !b[CTRLOP])
		ft_check_chr_syntax(b, line++, &popen, &pclose);
	if (b[QUOTE] || b[DQUOTE] || b[META] || b[CTRLOP] || popen != pclose)
		ft_put_error(ERROR_SYN, NULL);
	return ((b[QUOTE] || b[DQUOTE] || b[META] || b[CTRLOP] || popen != pclose));
}
