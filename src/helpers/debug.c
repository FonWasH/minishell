/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 06:29:25 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:09:22 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_get_exec_type_str(t_ctrlop type)
{
	if (type == NONE)
		return ("NONE");
	if (type == FIRST)
		return ("FIRST");
	if (type == SUB)
		return ("SUB");
	if (type == PIPE)
		return ("PIPE");
	if (type == AND)
		return ("AND");
	if (type == OR)
		return ("OR");
	return ("ERROR");
}

static void	ft_put_subline_tab(int sublevel)
{
	while (sublevel)
	{
		printf("\t");
		sublevel--;
	}
}

static void	ft_put_args(char **args, int sublevel)
{
	size_t	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		ft_put_subline_tab(sublevel);
		printf("ARG[%zu]:%s|\n", i, args[i]);
		i++;
	}
}

void	ft_test_line_parsing(t_list **cmds, int sublevel)
{
	t_cmd_data	*data;
	t_list		*tmp;

	tmp = *cmds;
	while (tmp)
	{
		printf("\n");
		data = (t_cmd_data *)tmp->data;
		ft_put_subline_tab(sublevel);
		printf("TYPE: %s\n", ft_get_exec_type_str(data->type));
		ft_put_subline_tab(sublevel);
		printf("NAME:%s|\n", data->name);
		ft_put_args(data->args, sublevel);
		ft_put_subline_tab(sublevel);
		printf("LIMITER:%s|\n", data->limiter);
		ft_put_subline_tab(sublevel);
		printf("INFILE:%s|\n", data->infile);
		ft_put_subline_tab(sublevel);
		printf("OUTFILE:%s|\n", data->outfile);
		if (data->sub)
			ft_test_line_parsing(&data->sub, sublevel + 1);
		tmp = tmp->next;
	}
}
