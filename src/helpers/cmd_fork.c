/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_fork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 19:45:32 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:09:09 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_redirect_fd(t_data *d, t_list *cmd, int *fd)
{
	t_cmd_data	*data;
	t_cmd_data	*data_next;

	data = (t_cmd_data *)cmd->data;
	data_next = NULL;
	if (cmd->next)
		data_next = (t_cmd_data *)cmd->next->data;
	close(fd[0]);
	if (data->infd != -1 && dup2(data->infd, STDIN_FILENO) == ERROR)
		ft_error(d, strerror(errno), NULL);
	if (data->outfd != -1 && dup2(data->outfd, STDOUT_FILENO) == ERROR)
		ft_error(d, strerror(errno), NULL);
	if (data_next && data_next->type == PIPE && data->outfd == -1
		&& dup2(fd[1], STDOUT_FILENO) == ERROR)
		ft_error(d, strerror(errno), NULL);
	close(fd[1]);
}

static void	ft_child(t_data *d, t_list *cmd, int *fd)
{
	t_cmd_data	*data;

	ft_signal_child(d);
	ft_redirect_fd(d, cmd, fd);
	data = (t_cmd_data *)cmd->data;
	if (data->builtin != E_NONE)
	{
		ft_exec_builtin(d, data);
		ft_exit(d, NULL, data->status);
	}
	else
	{
		if (!ft_exec(d, data))
			ft_error(d, NULL, NULL);
	}
}

static bool	ft_can_wait(t_cmd_data *data, t_cmd_data *data_next)
{
	t_cmd_data	*data_sub;

	data_sub = NULL;
	if (data->sub)
		data_sub = (t_cmd_data *)data->sub->data;
	return ((!data_sub && data_next && (data_next->type == OR
				|| data_next->type == AND)) || (data_sub
			&& (data_sub->type == OR || data_sub->type == AND)));
}

void	ft_exec_fork_cmd(t_data *d, t_list *cmd)
{
	int			fd[2];
	t_cmd_data	*data;
	t_cmd_data	*data_next;

	data = (t_cmd_data *)cmd->data;
	data_next = NULL;
	if (cmd->next)
		data_next = (t_cmd_data *)cmd->next->data;
	data->pid = ft_pipe_fork(d, fd);
	if (data->pid == 0)
		ft_child(d, cmd, fd);
	else
	{
		close(fd[1]);
		if (data_next && data_next->type == PIPE && data_next->infd == -1)
			data_next->infd = fd[0];
		else
			close(fd[0]);
		if (data->infd != -1)
			close(data->infd);
		if (ft_can_wait(data, data_next))
		{
			data->status = ft_waitpid(data->pid);
			d->exitcode = data->status;
		}
	}
}
