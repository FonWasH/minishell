/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 05:41:28 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:07:58 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	ft_check_identifier(char *line)
{
	int	i;

	if (line[0] != '_' && !ft_isalpha(line[0]))
		return (ft_put_error(ERROR_EXP, line), false);
	i = 0;
	while (line[i])
	{
		if (!ft_isalnum(line[i]) && line[i] != '_')
			return (ft_put_error(ERROR_EXP, line), false);
		i++;
	}
	return (true);
}

int	ft_unset(t_data *d, char **args)
{
	size_t	i;

	i = 0;
	while (args[i])
	{
		if (ft_check_identifier(args[i]))
		{
			if (!ft_unset_env(d, args[i]))
				return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
