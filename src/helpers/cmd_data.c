/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 20:40:21 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:08:51 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_init_cmd(t_cmd_data *cmd)
{
	cmd->line = NULL;
	cmd->name = NULL;
	cmd->args = NULL;
	cmd->heredoc = false;
	cmd->limiter = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->infd = -1;
	cmd->outfd = -1;
	cmd->outfileflags = -1;
	cmd->status = -1;
	cmd->type = NONE;
	cmd->builtin = E_NONE;
	cmd->pid = -1;
	cmd->sub = NULL;
	cmd->substatus = -1;
}

static void	ft_clear_cmd(t_cmd_data *cmd)
{
	if (cmd->pid > 0)
		kill(cmd->pid, SIGKILL);
	free(cmd->line);
	free(cmd->name);
	if (cmd->args)
		ft_mfree((void **)cmd->args, ft_mstrlen(cmd->args));
	free(cmd->limiter);
	free(cmd->infile);
	free(cmd->outfile);
	if (cmd->infd != -1)
		close(cmd->infd);
	if (cmd->outfd != -1)
		close(cmd->outfd);
}

void	ft_clear_cmds(t_list **cmds)
{
	t_cmd_data	*data;
	t_list		*tmp;

	tmp = *cmds;
	while (tmp)
	{
		data = (t_cmd_data *)tmp->data;
		if (data->sub)
			ft_clear_cmds(&data->sub);
		ft_clear_cmd(data);
		tmp = tmp->next;
	}
	ft_lstclear(cmds, free);
}
