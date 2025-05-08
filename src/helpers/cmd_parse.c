/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 20:40:36 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:09:13 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_file_name_end(char *str, size_t *end)
{
	while (str[*end] && ft_isspace(str[*end]))
		*end += 1;
	if (str[*end] == '\"')
	{
		*end += 1;
		while (str[*end] && str[*end] != '\"')
			*end += 1;
	}
	else if (str[*end] == '\'')
	{
		*end += 1;
		while (str[*end] && str[*end] != '\'')
			*end += 1;
	}
	else
	{
		while (str[*end] && !ft_isspace(str[*end + 1]))
			*end += 1;
	}
}

static void	ft_get_infile(t_data *d, t_cmd_data *cmd, size_t i, char **infile)
{
	size_t	start;
	size_t	end;

	if (*infile)
		free(*infile);
	cmd->heredoc = (&cmd->limiter == infile);
	start = i + 1 + cmd->heredoc;
	end = start;
	ft_file_name_end(cmd->line, &end);
	*infile = ft_substr(cmd->line, start, end - start + 1);
	if (!*infile)
		ft_error(d, ERROR_MEM, NULL);
	if (!cmd->line[end])
		ft_bzero(cmd->line + i, ft_strlen(cmd->line + i));
	else
		ft_memmove(cmd->line + i, cmd->line + end + 1,
			ft_strlen(cmd->line + end));
	ft_str_despace(*infile);
	if (!cmd->heredoc)
		ft_open_cmd_infile(d, cmd);
}

static void	ft_get_outfile(t_data *d, t_cmd_data *cmd, size_t i, t_openf flag)
{
	size_t	start;
	size_t	end;

	if (cmd->outfile)
		free(cmd->outfile);
	start = i + 1 + (flag == WR_APPEND);
	end = start;
	ft_file_name_end(cmd->line, &end);
	if (flag == WR_APPEND)
		cmd->outfileflags = O_WRONLY | O_CREAT | O_APPEND;
	else
		cmd->outfileflags = O_WRONLY | O_CREAT | O_TRUNC;
	cmd->outfile = ft_substr(cmd->line, start, end - start + 1);
	if (!cmd->outfile)
		ft_error(d, ERROR_MEM, NULL);
	if (!cmd->line[end])
		ft_bzero(cmd->line + i, ft_strlen(cmd->line + i));
	else
		ft_memmove(cmd->line + i, cmd->line + end + 1,
			ft_strlen(cmd->line + end));
	ft_str_despace(cmd->outfile);
	ft_open_cmd_outfile(d, cmd);
}

static void	ft_get_cmd_name(t_cmd_data *cmd)
{
	size_t	i;

	i = 0;
	while (cmd->line[i] && !ft_isspace(cmd->line[i]))
	{
		ft_skip_quotes(cmd->line, &i);
		if (!cmd->line[i])
			break ;
		i++;
	}
	cmd->name = ft_substr(cmd->line, 0, i + 1);
	if (!cmd->name)
		ft_error(NULL, ERROR_MEM, NULL);
	if (!cmd->line[i])
		ft_bzero(cmd->line, ft_strlen(cmd->line));
	else
		ft_memmove(cmd->line, cmd->line + i + 1,
			ft_strlen(cmd->line + i));
	ft_str_despace(cmd->name);
}

void	ft_parse_cmd(t_data *d, t_cmd_data *cmd)
{
	size_t	i;

	i = 0;
	while (cmd->line[i])
	{
		ft_skip_quotes(cmd->line, &i);
		if (cmd->line[i] == '<' && cmd->line[i + 1] == '<')
			ft_get_infile(d, cmd, i, &cmd->limiter);
		else if (cmd->line[i] == '<')
			ft_get_infile(d, cmd, i, &cmd->infile);
		if (cmd->line[i] == '>' && cmd->line[i + 1] == '>')
			ft_get_outfile(d, cmd, i, WR_APPEND);
		else if (cmd->line[i] == '>')
			ft_get_outfile(d, cmd, i, WR_TRUNC);
		i++;
	}
	ft_str_despace(cmd->line);
		cmd->line = ft_str_wildcard(cmd->line);
	if (!cmd->line)
		ft_error(NULL, ERROR_MEM, NULL);
	ft_get_cmd_name(cmd);
	cmd->args = ft_get_cmd_args(cmd->line, cmd->name);
	if (!cmd->args)
		ft_error(d, ERROR_MEM, NULL);
}
