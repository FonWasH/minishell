/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termios.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 14:50:24 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:14:43 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_init_term(t_data *d)
{
	if (tcgetattr(STDIN_FILENO, &d->term) == ERROR)
	{
		ft_put_error(strerror(errno), NULL);
		return (false);
	}
	return (true);
}

bool	ft_reset_term(t_data *d)
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, &d->term) == ERROR)
	{
		ft_put_error(strerror(errno), NULL);
		return (false);
	}
	return (true);
}
