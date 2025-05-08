/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 20:31:40 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:09:29 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_check_subline_ctrlop(t_linep *p)
{
	while (*(p->line) && *(p->line) != '&' && *(p->line) != '|')
		p->line++;
	if (*(p->line) && *(p->line) == '&' && *(p->line + 1) == '&')
	{
		p->type = AND;
		p->line += 2;
	}
	else if (*(p->line) && *(p->line) == '|' && *(p->line + 1) == '|')
	{
		p->type = OR;
		p->line += 2;
	}
	else if (*(p->line) && *(p->line) == '|')
	{
		p->type = PIPE;
		p->line++;
	}
	p->i = -1;
}

static void	ft_split_subline(t_data *d, t_list **cmds, t_linep *p)
{
	char	*sub;
	char	*start;
	char	*end;
	t_list	*last;

	start = ft_strchr(p->line, '(');
	end = ft_strrchr(p->line, ')');
	if (start && end && start < end)
	{
		sub = ft_substr(p->line, start - p->line + 1, end - start - 1);
		if (!sub)
			ft_error(d, ERROR_MEM, NULL);
		last = ft_lstlast(*cmds);
		if (((t_cmd_data *)last->data)->type == NONE)
			((t_cmd_data *)last->data)->type = SUB;
		ft_parse_line(d, &((t_cmd_data *)last->data)->sub, sub, p->type);
	}
	p->line = end + 1;
	ft_check_subline_ctrlop(p);
}

static void	ft_add_cmd(t_data *d, t_list **cmds, t_linep *p, int end)
{
	t_list		*last;
	t_cmd_data	*new;

	last = ft_lstlast(*cmds);
	if (((t_cmd_data *)last->data)->type == NONE)
		new = last->data;
	else
	{
		new = (t_cmd_data *)malloc(sizeof(t_cmd_data));
		if (!new)
			ft_error(d, ERROR_MEM, NULL);
		ft_init_cmd(new);
		ft_lstadd_back(cmds, ft_lstnew(new));
	}
	new->type = p->type;
	new->line = ft_substr(p->line, 0, end);
	if (!new->line)
		ft_error(d, ERROR_MEM, NULL);
	ft_parse_cmd(d, new);
}

static void	ft_check_line_ctrlop(t_data *d, t_list **cmds, t_linep *p)
{
	if (p->i > 0 && p->line[p->i] == '&' && p->line[p->i + 1] == '&')
	{
		ft_add_cmd(d, cmds, p, p->i);
		p->type = AND;
		p->line += p->i + 2;
		p->i = -1;
	}
	else if (p->i > 0 && p->line[p->i] == '|' && p->line[p->i + 1] == '|')
	{
		ft_add_cmd(d, cmds, p, p->i);
		p->type = OR;
		p->line += p->i + 2;
		p->i = -1;
	}
	else if (p->i > 0 && p->line[p->i] == '|')
	{
		ft_add_cmd(d, cmds, p, p->i);
		p->type = PIPE;
		p->line += p->i + 1;
		p->i = -1;
	}
	else if (p->line[p->i + 1] == '\0')
		ft_add_cmd(d, cmds, p, p->i + 1);
}

void	ft_parse_line(t_data *d, t_list **cmds, char *line, t_ctrlop type)
{
	t_linep		p;
	t_cmd_data	*new;

	p.i = 0;
	p.line = line;
	p.type = type;
	new = (t_cmd_data *)malloc(sizeof(t_cmd_data));
	if (!new)
		ft_error(d, ERROR_MEM, NULL);
	ft_init_cmd(new);
	ft_lstadd_back(cmds, ft_lstnew(new));
	while (p.line[p.i])
	{
		ft_skip_quotes(p.line, &p.i);
		if (p.line[p.i] == '(')
			ft_split_subline(d, cmds, &p);
		else
			ft_check_line_ctrlop(d, cmds, &p);
		p.i++;
	}
	free(line);
}
