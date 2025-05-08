/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 22:22:41 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:14:35 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_str_tilde(t_data *d, char *str)
{
	size_t	i;
	char	*home;
	char	*new;

	i = 0;
	while (str[i])
	{
		ft_skip_quotes(str, &i);
		if (str[i] == '~'
			&& (!str[i + 1] || ft_isspace(str[i + 1]) || str[i + 1] == '/')
			&& ((i > 0 && ft_isspace(str[i - 1])) || i == 0))
		{
			home = ft_get_env(d, HOME);
			if (!home)
				return (NULL);
			new = ft_strreplace(str, home, i, 1);
			if (!new)
				return (ft_put_error(ERROR_MEM, NULL), NULL);
			free(str);
			str = new;
			i += ft_strlen(home) - 1;
		}
		i++;
	}
	return (str);
}

static char	*ft_get_var(t_data *d, char *str, size_t *len)
{
	size_t	i;
	char	end;
	char	*value;

	if (!ft_strncmp(str, "$?", 2))
	{
		value = ft_ltoa(d->exitcode);
		*len = 2;
		return (value);
	}
	i = 1;
	while (str[i] && !ft_isspace(str[i]) && !ft_isquote(str[i])
		&& str[i] != '$' && str[i] != '/')
		i++;
	end = str[i];
	str[i] = '\0';
	value = ft_get_env(d, str + 1);
	if (len)
		*len = ft_strlen(str);
	str[i] = end;
	if (!value)
		value = "";
	return (ft_strdup(value));
}

static char	*ft_str_var(t_data *d, char *str)
{
	t_strp	p;

	p.dquote = false;
	p.i = -1;
	while (str[++p.i])
	{
		if (str[p.i] == '\"')
			p.dquote = !p.dquote;
		if (!p.dquote)
			ft_skip_quote(str, &p.i);
		if (ft_isvar(str, p.i))
		{
			p.var = ft_get_var(d, str + p.i, &p.len);
			if (!p.var)
				return (ft_put_error(ERROR_MEM, NULL), NULL);
			p.new = ft_strreplace(str, p.var, p.i, p.len);
			if (!p.new)
				return (free(p.var), ft_put_error(ERROR_MEM, NULL), NULL);
			p.i += ft_strlen(p.var) - 1;
			free(p.var);
			free(str);
			str = p.new;
		}
	}
	return (str);
}

static char	*ft_str_dequote(char *str)
{
	size_t	i;
	bool	quote;
	bool	dquote;

	i = 0;
	quote = false;
	dquote = false;
	while (str[i])
	{
		if (!quote && str[i] == '\"')
		{
			dquote = !dquote;
			ft_memmove(str + i, str + i + 1, ft_strlen(str + i));
		}
		else if (!dquote && str[i] == '\'')
		{
			quote = !quote;
			ft_memmove(str + i, str + i + 1, ft_strlen(str + i));
		}
		else
			i++;
	}
	return (str);
}

char	*ft_str_expand(t_data *d, char *str)
{
	char	*clean;

	if (!str)
		return (NULL);
	clean = ft_str_tilde(d, str);
	if (!clean)
		return (NULL);
	clean = ft_str_var(d, clean);
	if (!clean)
		return (NULL);
	clean = ft_str_dequote(clean);
	return (clean);
}
