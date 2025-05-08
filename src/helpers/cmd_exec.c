/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 21:31:14 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:08:54 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	ft_can_exec(t_data *d, t_list *cmd)
{
	t_cmd_data	*data;
	t_cmd_data	*data_prev;

	data = (t_cmd_data *)cmd->data;
	if (data->status != -1)
	{
		d->exitcode = data->status;
		return (false);
	}
	data_prev = NULL;
	if (cmd->prev)
		data_prev = (t_cmd_data *)cmd->prev->data;
	if (data_prev && ((!data_prev->sub && ((data->type == AND
						&& data_prev->status != EXIT_SUCCESS)
					|| (data->type == OR && data_prev->status == EXIT_SUCCESS)))
			|| (data_prev->sub && ((data->type == AND
						&& data_prev->substatus != EXIT_SUCCESS)
					|| (data->type == OR
						&& data_prev->substatus == EXIT_SUCCESS)))))
		return (false);
	return (true);
}

static void	ft_exec_cmd(t_data *d, t_list *cmd)
{
	t_cmd_data	*data;
	t_cmd_data	*data_next;
	t_cmd_data	*data_sub;

	data = (t_cmd_data *)cmd->data;
	data_next = NULL;
	if (cmd->next)
		data_next = (t_cmd_data *)cmd->next->data;
	data_sub = NULL;
	if (data->sub)
		data_sub = (t_cmd_data *)data->sub->data;
	if (!ft_can_exec(d, cmd))
		return ;
	if (data->type == PIPE || data->builtin == E_NONE || (!data_sub && data_next
			&& data_next->type == PIPE) || (data_sub && data_sub->type == PIPE))
		ft_exec_fork_cmd(d, cmd);
	else
	{
		ft_exec_builtin(d, data);
		d->exitcode = data->status;
	}
}

static bool	ft_can_wait(t_list *cmd)
{
	t_cmd_data	*data;
	t_cmd_data	*data_next;
	t_cmd_data	*data_sub;

	data = (t_cmd_data *)cmd->data;
	if (data->status == EXIT_FAILURE)
		return (false);
	data_next = NULL;
	if (cmd->next)
		data_next = (t_cmd_data *)cmd->next->data;
	data_sub = NULL;
	if (data->sub)
		data_sub = (t_cmd_data *)data->sub->data;
	return ((!data_sub && (((data->type == PIPE || data->builtin == E_NONE)
					&& ((data_next && data_next->type != AND
							&& data_next->type != OR) || !data_next))
				|| (data_next && data_next->type == PIPE))) || (data_sub
			&& (data->type == PIPE || data->builtin == E_NONE)
			&& data_sub->type != AND && data_sub->type != OR));
}

static void	ft_wait_cmds(t_data *d, t_list **cmds)
{
	t_list		*tmp;
	t_cmd_data	*data;

	tmp = *cmds;
	while (tmp)
	{
		data = (t_cmd_data *)tmp->data;
		if (ft_can_wait(tmp))
		{
			data->status = ft_waitpid(data->pid);
			d->exitcode = data->status;
		}
		tmp = tmp->next;
	}
}

void	ft_exec_cmds(t_data *d, t_list **cmds)
{
	t_list		*tmp;
	t_cmd_data	*data;

	ft_signal_sigint(d);
	ft_heredoc_cmds(d, cmds);
	ft_signal_sigquit(d);
	tmp = *cmds;
	while (tmp)
	{
		data = (t_cmd_data *)tmp->data;
		ft_expand_cmd(d, data);
		if (data->name && *(data->name))
			ft_exec_cmd(d, tmp);
		if (data->sub)
			ft_subshell(d, data, &data->sub);
		tmp = tmp->next;
	}
	ft_wait_cmds(d, cmds);
	ft_signal(d);
}
