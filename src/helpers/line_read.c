/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_read.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 20:04:09 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:09:35 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_concate_prompt(char *user, char *host, char *pwd)
{
	size_t		i;
	char		*prompt;
	char		*tmp;
	const char	*parts[] = {
		COLOR_GREEN, user, "@", host, COLOR_RESET, ":",
		COLOR_BLUE, pwd, COLOR_RESET, "$ "};

	i = 0;
	prompt = "";
	while (i < 10)
	{
		tmp = ft_strjoin(prompt, (char *)parts[i]);
		if (*prompt)
			free(prompt);
		if (!tmp)
			return (free(pwd), free(host), ft_put_error(ERROR_MEM, NULL), NULL);
		prompt = tmp;
		i++;
	}
	return (free(pwd), free(host), prompt);
}

static char	*ft_get_hostname(void)
{
	int		fd;
	char	*line;
	char	*hostname;

	fd = open(PATH_HTN, 0, O_RDONLY);
	if (fd == ERROR)
		return (close(fd), ft_put_error(strerror(errno), PATH_HTN), NULL);
	line = get_next_line(fd);
	if (!line)
		return (close(fd), NULL);
	line[ft_strlen(line) - 1] = '\0';
	hostname = ft_strdup(line);
	while (line)
	{
		free(line);
		line = get_next_line(fd);
	}
	return (close(fd), hostname);
}

static char	*ft_get_pwd(t_data *d)
{
	char	*pwd;
	char	*home;
	char	*path;

	pwd = ft_get_env(d, PWD);
	if (!pwd)
		return (NULL);
	home = ft_get_env(d, HOME);
	if (!home)
		return (NULL);
	if (!ft_strncmp(pwd, home, ft_strlen(home)))
		path = ft_strjoin("~", pwd + ft_strlen(home));
	else
		path = ft_strdup(pwd);
	return (path);
}

static char	*ft_get_prompt(t_data *d)
{
	char	*username;
	char	*hostname;
	char	*pwd;

	username = ft_get_env(d, USER);
	if (!username)
		return (NULL);
	hostname = ft_get_hostname();
	if (!hostname)
		return (NULL);
	pwd = ft_get_pwd(d);
	if (!pwd)
		return (free(hostname), NULL);
	return (ft_concate_prompt(username, hostname, pwd));
}

char	*ft_read_line(t_data *d)
{
	char	*prompt;
	char	*line;

	prompt = ft_get_prompt(d);
	if (prompt)
	{
		line = readline(prompt);
		free(prompt);
	}
	else
		line = readline(PROMPT);
	if (line && *line)
		add_history(line);
	return (line);
}
