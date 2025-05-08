/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 07:23:20 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:10:23 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	ft_pipe_fork(t_data *d, int *fd)
{
	pid_t	pid;

	if (pipe(fd) == ERROR)
		return (ft_error(d, strerror(errno), NULL), ERROR);
	pid = fork();
	if (pid == ERROR)
		return (close(fd[0]), close(fd[1]),
			ft_error(d, strerror(errno), NULL), ERROR);
	return (pid);
}

pid_t	ft_fork(t_data *d)
{
	pid_t	pid;

	pid = fork();
	if (pid == ERROR)
		return (ft_error(d, strerror(errno), NULL), ERROR);
	return (pid);
}
