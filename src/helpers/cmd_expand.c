/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 18:09:42 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:08:58 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_expand_cmd_args(t_data *d, t_cmd_data *cmd)
{
	size_t	i;

	if (!cmd->args)
		return ;
	i = 0;
	while (cmd->args[i])
	{
		cmd->args[i] = ft_str_expand(d, cmd->args[i]);
		if (!cmd->args[i])
			ft_error(d, NULL, NULL);
		i++;
	}
}

static void	ft_expand_cmd_name(t_data *d, t_cmd_data *cmd)
{
	if (cmd->name)
	{
		cmd->name = ft_str_expand(d, cmd->name);
		if (!cmd->name)
			ft_error(d, NULL, NULL);
	}
}

void	ft_expand_cmd(t_data *d, t_cmd_data *cmd)
{
	ft_expand_cmd_name(d, cmd);
	ft_expand_cmd_args(d, cmd);
	if (cmd->name)
		cmd->builtin = ft_isbuiltin(cmd->name);
}
