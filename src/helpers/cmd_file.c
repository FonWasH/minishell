/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_file.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 20:40:39 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:09:03 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_open_cmd_infile(t_data *d, t_cmd_data *cmd)
{
	if (cmd->infile && cmd->status == -1)
	{
		cmd->infile = ft_str_expand(d, cmd->infile);
		if (!cmd->infile)
			ft_error(d, NULL, NULL);
		if (cmd->infd != -1)
			close(cmd->infd);
		cmd->infd = open(cmd->infile, O_RDONLY, 0);
		if (cmd->infd == ERROR)
		{
			ft_put_error(strerror(errno), cmd->infile);
			cmd->status = EXIT_NOT_FOUND;
		}
	}
}

void	ft_open_cmd_outfile(t_data *d, t_cmd_data *cmd)
{
	if (cmd->outfile && cmd->status == -1)
	{
		cmd->outfile = ft_str_expand(d, cmd->outfile);
		if (!cmd->outfile)
			ft_error(d, NULL, NULL);
		if (cmd->outfd != -1)
			close(cmd->outfd);
		cmd->outfd = open(cmd->outfile, cmd->outfileflags, 0644);
		if (cmd->outfd == ERROR)
		{
			ft_put_error(strerror(errno), cmd->outfile);
			cmd->status = EXIT_NOT_FOUND;
		}
	}
}

static void	ft_child(t_data *d, int *fd, char *eof)
{
	char	*line;

	ft_signal_heredoc(d);
	close(fd[0]);
	while (true)
	{
		line = readline("> ");
		if (!line)
		{
			ft_put_error(ERROR_EOF, eof);
			break ;
		}
		if (!ft_strncmp(line, eof, ft_strlen(eof) + 1))
		{
			free(line);
			break ;
		}
		ft_putstr(line, fd[1]);
		ft_putstr("\n", fd[1]);
		free(line);
	}
	close(fd[1]);
	ft_exit(d, NULL, EXIT_SUCCESS);
}

static void	ft_heredoc(t_data *d, t_cmd_data *cmd)
{
	int		fd[2];
	pid_t	pid;

	pid = ft_pipe_fork(d, fd);
	if (pid == 0)
		ft_child(d, fd, cmd->limiter);
	else
	{
		close(fd[1]);
		if (cmd->heredoc)
			cmd->infd = fd[0];
		ft_waitpid(pid);
		if (!cmd->heredoc)
			close(fd[0]);
	}
}

void	ft_heredoc_cmds(t_data *d, t_list **cmds)
{
	t_list		*tmp;
	t_cmd_data	*data;

	tmp = *cmds;
	while (tmp)
	{
		data = (t_cmd_data *)tmp->data;
		if (data->limiter)
			ft_heredoc(d, data);
		tmp = tmp->next;
	}
}
