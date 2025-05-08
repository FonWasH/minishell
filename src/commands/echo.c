/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 05:31:09 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:07:38 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_option_valid(char *line)
{
	bool	is_new_line;
	size_t	i;

	is_new_line = (line[0] && line[0] == '-' && line[1] && line[1] == 'n');
	if (is_new_line)
	{
		i = 2;
		while (line[i])
		{
			if (line[i] != 'n')
				return (false);
			i++;
		}
	}
	return (is_new_line);
}

int	ft_echo(char **args)
{
	size_t	i;
	bool	new_line;
	bool	first;

	i = 0;
	new_line = true;
	first = true;
	while (args[i])
	{
		if (first && is_option_valid(args[i]))
			new_line = false;
		else if (first)
		{
			first = false;
			printf("%s", args[i]);
		}
		else
			printf(" %s", args[i]);
		i++;
	}
	if (new_line)
		printf("\n");
	return (EXIT_SUCCESS);
}
