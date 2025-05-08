/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 05:38:47 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:07:50 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_put_all_env(t_data *d)
{
	char	**env;
	size_t	i;

	env = ft_get_env_map(d, true);
	if (!env)
		return (EXIT_FAILURE);
	ft_sortmstr(env);
	i = 0;
	while (env[i])
	{
		printf("declare -x %s\n", env[i]);
		i++;
	}
	return (ft_mfree((void **)env, ft_mstrlen(env)), EXIT_SUCCESS);
}

static bool	ft_check_identifier(char *line)
{
	int	i;

	if (line[0] != '_' && !ft_isalpha(line[0]))
		return (ft_put_error(ERROR_EXP, line), false);
	i = 0;
	while (line[i] && line[i] != '=')
	{
		if (!ft_isalnum(line[i]) && line[i] != '_')
			return (ft_put_error(ERROR_EXP, line), false);
		i++;
	}
	return (true);
}

static int	ft_export_args(t_data *d, char **args)
{
	size_t	i;
	char	*sep;
	int		ret;

	ret = EXIT_SUCCESS;
	i = 0;
	while (args[i])
	{
		if (ft_check_identifier(args[i]))
		{
			sep = ft_strchr(args[i], '=');
			if (sep)
			{
				*sep = '\0';
				if (!ft_set_env(d, args[i], sep + 1, true))
					return (EXIT_FAILURE);
			}
			else if (!ft_set_env(d, args[i], "", false))
				return (EXIT_FAILURE);
		}
		else
			ret = EXIT_FAILURE;
		i++;
	}
	return (ret);
}

int	ft_export(t_data *d, char **args)
{
	if (!args || !args[0] || !*args[0])
		return (ft_put_all_env(d));
	return (ft_export_args(d, args));
}
