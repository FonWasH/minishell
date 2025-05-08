/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 20:02:16 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:09:47 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_handler_sigint(int signum)
{
	(void)signum;
	g_sig_exitcode = EXIT_INTERRUPT;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	ft_handler_sigint_cmd(int signum)
{
	(void)signum;
	g_sig_exitcode = EXIT_INTERRUPT;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
}

void	ft_handler_sigquit_cmd(int signum)
{
	(void)signum;
	g_sig_exitcode = EXIT_QUIT;
	printf(QUIT);
}
