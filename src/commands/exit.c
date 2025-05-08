/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 05:37:08 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:07:46 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_number(char *line)
{
	size_t	i;

	i = 0;
	if (line[i] == '+' || line[i] == '-')
		i++;
	while (line[i])
	{
		if (!ft_isdigit(line[i]))
			return (false);
		i++;
	}
	return (true);
}

void	ft_check_exitcode(t_data *d, char **args)
{
	ssize_t	exitcode;

	if ((ft_strlen(args[0]) == 1 && (*args[0] == '0' || *args[0] == '1'))
		|| (ft_strlen(args[0]) == 2 && args[0][0] == '-' && args[0][1] == '1'))
		ft_exit(d, EXIT, (unsigned char)ft_atol(args[0]));
	else
	{
		exitcode = ft_atol(args[0]);
		if (exitcode == 0 || exitcode == -1)
		{
			ft_put_error(ERROR_NUM, NULL);
			ft_exit(d, EXIT, EXIT_INPUT);
		}
		else
			ft_exit(d, EXIT, (unsigned char)exitcode);
	}
}

void	ft_exit_cmd(t_data *d, char **args)
{
	if (!args || !args[0] || !*args[0])
		ft_exit(d, EXIT, EXIT_SUCCESS);
	if (!is_number(args[0]))
	{
		ft_put_error(ERROR_NUM, NULL);
		ft_exit(d, EXIT, EXIT_INPUT);
	}
	if (ft_mstrlen(args) > 1)
	{
		ft_put_error(ERROR_ARG, NULL);
		ft_exit(d, EXIT, EXIT_FAILURE);
	}
	ft_check_exitcode(d, args);
}

void	ft_exit(t_data *d, char *msg, int exitcode)
{
	if (msg)
		printf("%s", msg);
	ft_clear_data(d);
	exit(exitcode);
}
