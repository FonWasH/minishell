/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 23:08:51 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:10:19 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_free_find(char *cmd, char **paths, bool error)
{
	if (error)
		ft_put_error(ERROR_MEM, NULL);
	if (cmd)
		free(cmd);
	if (paths)
		ft_mfree((void **)paths, ft_mstrlen(paths));
}

static char	*ft_find_cmd(t_data *d, char *cmd)
{
	char	*env_path;
	char	*cmd_path;
	char	**paths;
	size_t	i;

	env_path = ft_get_env(d, PATH);
	if (!env_path)
		return (ft_free_find(cmd, NULL, false), NULL);
	paths = ft_split(env_path, ':');
	if (!paths)
		return (ft_free_find(cmd, NULL, true), NULL);
	i = 0;
	while (paths[i])
	{
		cmd_path = ft_strjoin(paths[i], cmd);
		if (!cmd_path)
			return (ft_free_find(cmd, paths, true), NULL);
		if (!ft_ispath_dir(cmd_path) && ft_ispath_exec(cmd_path)
			&& !access(cmd_path, F_OK))
			return (ft_free_find(cmd, paths, false), cmd_path);
		free(cmd_path);
		i++;
	}
	return (ft_free_find(cmd, paths, false), NULL);
}

static char	*ft_get_cmd_path(t_data *d, char **cmd_args)
{
	char	*cmd;
	char	*cmd_path;

	if (ft_ispath(cmd_args[0]) && !access(cmd_args[0], F_OK))
	{
		if (ft_ispath_dir(cmd_args[0]))
			return (ft_put_error(ERROR_DIR, cmd_args[0]),
				ft_exit(d, NULL, EXIT_PERMISSION), NULL);
		cmd_path = ft_strdup(cmd_args[0]);
		if (!*cmd_path)
			return (ft_put_error(ERROR_MEM, NULL), NULL);
		return (cmd_path);
	}
	cmd = ft_strjoin("/", cmd_args[0]);
	if (!cmd)
		return (ft_put_error(ERROR_MEM, NULL), NULL);
	cmd_path = ft_find_cmd(d, cmd);
	if (!cmd_path)
		return (ft_put_error(ERROR_CMD, cmd_args[0]),
			ft_exit(d, NULL, EXIT_NOT_FOUND), NULL);
	return (cmd_path);
}

static void	ft_free_exec(t_data *d, char *path, char **env, char *error)
{
	if (error)
		ft_put_error(error, path);
	if (path)
		free(path);
	if (env)
		ft_mfree((void **)env, ft_mstrlen(env));
	if (error)
		ft_exit(d, NULL, EXIT_PERMISSION);
}

bool	ft_exec(t_data *d, t_cmd_data *cmd)
{
	char	*cmd_path;
	char	**env;

	if (!*cmd->name)
		return (ft_put_error(ERROR_CMD, cmd->name),
			ft_exit(d, NULL, EXIT_NOT_FOUND), false);
	cmd_path = ft_get_cmd_path(d, cmd->args);
	if (!cmd_path)
		return (false);
	env = ft_get_env_map(d, false);
	if (!env)
		return (free(cmd_path), false);
	if (execve(cmd_path, cmd->args, env) == ERROR)
		return (ft_free_exec(d, cmd_path, env, strerror(errno)), false);
	return (ft_free_exec(d, cmd_path, env, NULL), true);
}
