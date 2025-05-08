/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 06:20:17 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:08:15 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_line(t_data *d)
{
	if (!ft_check_line_syntax(d->line))
	{
		ft_parse_line(d, &d->cmds, d->line, FIRST);
		d->line = NULL;
		if (DEBUG)
			ft_test_line_parsing(&d->cmds, 0);
		ft_exec_cmds(d, &d->cmds);
		ft_clear_cmds(&d->cmds);
		ft_reset_term(d);
	}
	else
	{
		d->exitcode = EXIT_INPUT;
		free(d->line);
		d->line = NULL;
	}
}

int	minishell(void)
{
	t_data	d;

	ft_init_data(&d);
	ft_signal(&d);
	while (true)
	{
		d.line = ft_read_line(&d);
		if (g_sig_exitcode != EXIT_SUCCESS)
		{
			d.exitcode = g_sig_exitcode;
			g_sig_exitcode = EXIT_SUCCESS;
		}
		if (!d.line)
			ft_exit(&d, EXIT, EXIT_SUCCESS);
		if (d.line && *d.line)
			ft_line(&d);
	}
	return (EXIT_SUCCESS);
}
