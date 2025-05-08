/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 20:23:36 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:09:51 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_signal(t_data *d)
{
	if (signal(SIGINT, ft_handler_sigint) == SIG_ERR)
		ft_error(d, ERROR_SIG, NULL);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		ft_error(d, ERROR_SIG, NULL);
}

void	ft_signal_sigint(t_data *d)
{
	if (signal(SIGINT, ft_handler_sigint_cmd) == SIG_ERR)
		ft_error(d, ERROR_SIG, NULL);
}

void	ft_signal_sigquit(t_data *d)
{
	if (signal(SIGQUIT, ft_handler_sigquit_cmd) == SIG_ERR)
		ft_error(d, ERROR_SIG, NULL);
}

void	ft_signal_child(t_data *d)
{
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		ft_error(d, ERROR_SIG, NULL);
}

void	ft_signal_heredoc(t_data *d)
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		ft_error(d, ERROR_SIG, NULL);
}
