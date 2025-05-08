/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_subshell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 04:25:29 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:09:17 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	ft_can_exec(t_cmd_data *parent_cmd, t_list *cmd)
{
	t_cmd_data	*data;

	data = (t_cmd_data *)cmd->data;
	if (parent_cmd->type != SUB && ((data->type == AND
				&& parent_cmd->status != EXIT_SUCCESS) || (data->type == OR
				&& parent_cmd->status == EXIT_SUCCESS)))
		return (false);
	return (true);
}

void	ft_subshell(t_data *d, t_cmd_data *parent_cmd, t_list **cmds)
{
	pid_t	pid;

	if (!ft_can_exec(parent_cmd, *cmds))
		return ;
	pid = ft_fork(d);
	if (pid == 0)
	{
		ft_exec_cmds(d, cmds);
		ft_exit(d, NULL, d->exitcode);
	}
	parent_cmd->substatus = ft_waitpid(pid);
	d->exitcode = parent_cmd->substatus;
}
