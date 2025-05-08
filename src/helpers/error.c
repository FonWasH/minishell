/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 06:28:17 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:09:25 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_put_error(char *error, char *extra)
{
	ft_putstr(NAME, STDERR_FILENO);
	ft_putstr(": ", STDERR_FILENO);
	ft_putstr(error, STDERR_FILENO);
	if (extra)
	{
		ft_putstr(": '", STDERR_FILENO);
		ft_putstr(extra, STDERR_FILENO);
		ft_putstr("'", STDERR_FILENO);
	}
	ft_putstr("\n", STDERR_FILENO);
}

void	ft_error(t_data *d, char *error, char *extra)
{
	if (error)
		ft_put_error(error, extra);
	ft_clear_data(d);
	exit(EXIT_FAILURE);
}
