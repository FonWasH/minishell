/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:20:33 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:08:32 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_arglen(char *line)
{
	size_t	i;

	i = 0;
	while (line[i] && !ft_isspace(line[i]))
	{
		ft_skip_quotes(line, &i);
		i++;
	}
	return (i);
}

static size_t	ft_countargs(char *line)
{
	bool	sep;
	size_t	count;
	size_t	i;

	sep = true;
	count = 0;
	i = 0;
	while (line[i])
	{
		ft_skip_quotes(line, &i);
		if (ft_isspace(line[i]))
			sep = true;
		if (!ft_isspace(line[i]) && sep)
		{
			sep = false;
			count++;
		}
		i++;
	}
	return (count);
}

static char	**ft_clean_args(char **args)
{
	size_t	i;

	if (!args)
		return (NULL);
	i = 0;
	while (args[i])
	{
		ft_str_despace(args[i]);
		i++;
	}
	return (args);
}

char	**ft_get_cmd_args(char *line, char *name)
{
	char	**args;
	size_t	count;
	size_t	len;
	size_t	i;

	count = ft_countargs(line) + 1;
	args = (char **)malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	args[0] = ft_strdup(name);
	if (!args[0])
		return (ft_mfree((void **)args, 0), NULL);
	i = 0;
	while (++i < count)
	{
		while (ft_isspace(*line))
			line++;
		len = ft_arglen(line);
		args[i] = ft_substr(line, 0, len);
		if (!args[i])
			return (ft_mfree((void **)args, i), NULL);
		line += len;
	}
	args[i] = NULL;
	return (ft_clean_args(args));
}
