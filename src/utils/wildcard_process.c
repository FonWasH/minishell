/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_process.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 22:39:43 by cbaillet          #+#    #+#             */
/*   Updated: 2025/05/07 15:14:51 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_update_string(char *str, char *files, int start, int len)
{
	char	*new;

	new = ft_strreplace(str, files, start, len);
	if (!new)
		return (ft_put_error(ERROR_MEM, NULL), NULL);
	free(str);
	ft_str_despace(new);
	return (new);
}

static void	ft_cleanup(DIR *dir, char *s1, char *s2)
{
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	if (closedir(dir) == ERROR)
		ft_put_error(strerror(errno), NULL);
}

static char	*ft_append_filename(char *files, char *filename)
{
	files = ft_strjoin_free(files, " ", 1);
	if (!files)
		return (NULL);
	files = ft_strjoin_free(files, filename, 1);
	if (!files)
		return (NULL);
	return (files);
}

static char	*ft_read_directory(DIR *dir, char *pattern)
{
	char			*files;
	struct dirent	*dirent;

	dirent = readdir(dir);
	if (!dirent)
		return (ft_put_error(strerror(errno), NULL), NULL);
	files = ft_strdup("");
	if (!files)
		return (ft_put_error(ERROR_MEM, NULL), NULL);
	while (dirent)
	{
		if (ft_is_valid_file(dirent) && ft_match(pattern, dirent->d_name))
		{
			files = ft_append_filename(files, dirent->d_name);
			if (!files)
				return (ft_put_error(ERROR_MEM, NULL), NULL);
		}
		dirent = readdir(dir);
	}
	return (files);
}

char	*ft_process_wildcard(char *str, char *pattern)
{
	DIR		*dir;
	char	*files;
	char	*start;

	dir = opendir(".");
	if (!dir)
		return (free(pattern), ft_put_error(strerror(errno), "."), NULL);
	start = ft_strnstr(str, pattern, ft_strlen(str));
	files = ft_read_directory(dir, pattern);
	if (!files)
		return (ft_cleanup(dir, pattern, NULL), NULL);
	if (*files)
		str = ft_update_string(str, files, start - str, ft_strlen(pattern));
	ft_cleanup(dir, pattern, files);
	return (str);
}
