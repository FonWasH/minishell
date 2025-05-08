/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 06:15:26 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:08:04 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_init_data(t_data *d)
{
	if (!ft_init_term(d))
		ft_error(d, NULL, NULL);
	d->env = NULL;
	if (!ft_init_env(d))
		ft_error(d, NULL, NULL);
	d->line = NULL;
	d->cmds = NULL;
	d->exitcode = EXIT_SUCCESS;
	g_sig_exitcode = EXIT_SUCCESS;
}

void	ft_clear_data(t_data *d)
{
	if (d->line)
	{
		free(d->line);
		d->line = NULL;
	}
	if (d->cmds)
	{
		ft_clear_cmds(&d->cmds);
		d->cmds = NULL;
	}
	if (d->env)
	{
		ft_clear_env(&d->env);
		d->env = NULL;
	}
}
