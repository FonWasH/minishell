/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 06:02:13 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:10:15 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bin	ft_isbuiltin(char *name)
{
	static const char	*builtin_str[8] = {
		B_CD, B_ECHO, B_ENV, B_EXIT, B_EXPORT, B_PWD, B_UNSET, NULL
	};
	static const t_bin	builtin_enum[7] = {
		E_CD, E_ECHO, E_ENV, E_EXIT, E_EXPORT, E_PWD, E_UNSET
	};
	size_t				len;
	size_t				i;

	i = 0;
	while (builtin_str[i])
	{
		len = ft_strlen((char *)builtin_str[i]) + 1;
		if (!ft_strncmp(name, (char *)builtin_str[i], len))
			return (builtin_enum[i]);
		i++;
	}
	return (E_NONE);
}

static bool	ft_save_fd(bool save)
{
	static int	saved_stdout;
	static int	saved_stdin;

	if (save)
	{
		saved_stdout = dup(STDOUT_FILENO);
		if (saved_stdout == ERROR)
			return (ft_put_error(strerror(errno), NULL), false);
		saved_stdin = dup(STDIN_FILENO);
		if (saved_stdin == ERROR)
			return (ft_put_error(strerror(errno), NULL),
				close(saved_stdout), false);
	}
	else
	{
		if (dup2(saved_stdout, STDOUT_FILENO) == ERROR)
			return (ft_put_error(strerror(errno), NULL),
				close(saved_stdout), close(saved_stdin), false);
		if (dup2(saved_stdin, STDIN_FILENO) == ERROR)
			return (ft_put_error(strerror(errno), NULL),
				close(saved_stdout), close(saved_stdin), false);
		return (close(saved_stdout), close(saved_stdin), true);
	}
	return (true);
}

static int	ft_redirect_fd(t_cmd_data *cmd)
{
	if (!ft_save_fd(true))
		return (ft_save_fd(false), EXIT_FAILURE);
	if (cmd->infd != -1 && dup2(cmd->infd, STDIN_FILENO) == ERROR)
		return (ft_put_error(strerror(errno), NULL),
			ft_save_fd(false), EXIT_FAILURE);
	if (cmd->outfd != -1 && dup2(cmd->outfd, STDOUT_FILENO) == ERROR)
		return (ft_put_error(strerror(errno), NULL),
			ft_save_fd(false), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

void	ft_exec_builtin(t_data *d, t_cmd_data *cmd)
{
	cmd->status = ft_redirect_fd(cmd);
	if (cmd->status == EXIT_FAILURE)
		return ;
	if (cmd->builtin == E_CD)
		cmd->status = ft_cd(d, cmd->args + 1);
	else if (cmd->builtin == E_ECHO)
		cmd->status = ft_echo(cmd->args + 1);
	else if (cmd->builtin == E_ENV)
		cmd->status = ft_env(d);
	else if (cmd->builtin == E_EXIT)
	{
		ft_save_fd(false);
		ft_exit_cmd(d, cmd->args + 1);
	}
	else if (cmd->builtin == E_EXPORT)
		cmd->status = ft_export(d, cmd->args + 1);
	else if (cmd->builtin == E_PWD)
		cmd->status = ft_pwd(d);
	else if (cmd->builtin == E_UNSET)
		cmd->status = ft_unset(d, cmd->args + 1);
	ft_save_fd(false);
}
