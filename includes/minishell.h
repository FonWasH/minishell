/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juperez <juperez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 21:26:38 by juperez           #+#    #+#             */
/*   Updated: 2025/05/07 15:21:16 by juperez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ************************************************************************** */
/*                                  INCLUDES                                  */
/* ************************************************************************** */

# include "libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>
# include <dirent.h>
# include <signal.h>
# include <string.h>
# include <errno.h>

/* ************************************************************************** */
/*                                  DEFINES                                   */
/* ************************************************************************** */

# define NAME			"minishell"

# define USER			"USER"
# define HOME			"HOME"
# define PATH			"PATH"
# define PWD			"PWD"
# define OLDPWD			"OLDPWD"

# define PATH_HTN		"/etc/hostname"

# define PROMPT			"\033[1;32mminishell\033[0m$ "
# define QUIT			"Quit\n"
# define EXIT			"exit\n"

# define B_ECHO			"echo"
# define B_CD			"cd"
# define B_PWD			"pwd"
# define B_ENV			"env"
# define B_UNSET		"unset"
# define B_EXPORT		"export"
# define B_EXIT			"exit"

# define COLOR_BLUE		"\001\033[1;34m\002"
# define COLOR_GREEN	"\001\033[1;32m\002"
# define COLOR_RESET	"\001\033[0m\002"

# define ERROR			-1
# define ERROR_SYN		"Syntax error"
# define ERROR_MEM		"Memory allocation failed"
# define ERROR_CMD		"Command not found"
# define ERROR_DIR		"Is a directory"
# define ERROR_FIL		"Filename argument required"
# define ERROR_SIG		"Failed to catch signal"
# define ERROR_ARG		"Too many arguments"
# define ERROR_NUM		"Numeric argument required"
# define ERROR_EXP		"Not a valid identifier"
# define ERROR_EOF		"Warning: Here-doc delimited by EOF, wanted"

# ifndef DEBUG
#  define DEBUG			0
# endif

/* ************************************************************************** */
/*                                   ENUMS                                    */
/* ************************************************************************** */

typedef enum e_ecode
{
	EXIT_INPUT = 2,
	EXIT_PERMISSION = 126,
	EXIT_NOT_FOUND = 127,
	EXIT_INTERRUPT = 130,
	EXIT_QUIT = 131,
}	t_ecode;

typedef enum e_opchar
{
	QUOTE,
	DQUOTE,
	META,
	CTRLOP
}	t_opchar;

typedef enum e_ctrlop
{
	NONE,
	FIRST,
	SUB,
	PIPE,
	AND,
	OR
}	t_ctrlop;

typedef enum e_openf
{
	WR_APPEND,
	WR_TRUNC,
	RD
}	t_openf;

typedef enum e_bin
{
	E_NONE,
	E_CD,
	E_ECHO,
	E_ENV,
	E_EXIT,
	E_EXPORT,
	E_PWD,
	E_UNSET
}	t_bin;

/* ************************************************************************** */
/*                                   GLOBAL                                   */
/* ************************************************************************** */

extern int						g_sig_exitcode;

/* ************************************************************************** */
/*                                  TYPEDEFS                                  */
/* ************************************************************************** */

typedef struct dirent			t_dire;
typedef struct stat				t_stat;
typedef struct termios			t_term;

/* ************************************************************************** */
/*                                 STRUCTURES                                 */
/* ************************************************************************** */

typedef struct s_strp
{
	char						*new;
	char						*var;
	size_t						i;
	size_t						len;
	bool						dquote;
}								t_strp;

typedef struct s_linep
{
	size_t						i;
	char						*line;
	t_ctrlop					type;
}								t_linep;

typedef struct s_env_data
{
	char						*key;
	char						*value;
	bool						exported;
}								t_env_data;

typedef struct s_cmd_data
{
	char						*line;
	char						*name;
	char						**args;
	bool						heredoc;
	char						*limiter;
	char						*infile;
	char						*outfile;
	int							infd;
	int							outfd;
	int							outfileflags;
	int							status;
	t_ctrlop					type;
	t_bin						builtin;
	pid_t						pid;
	t_list						*sub;
	int							substatus;
}								t_cmd_data;

typedef struct s_data
{
	t_term						term;
	char						*line;
	t_list						*cmds;
	t_list						*env;
	int							exitcode;
}								t_data;

/* ************************************************************************** */
/*                                 FUNCTIONS                                  */
/* ************************************************************************** */

/* CORE ********************************************************************* */

int		minishell(void);
void	ft_init_data(t_data *d);
void	ft_clear_data(t_data *d);

/* HELPERS ****************************************************************** */

// debug

void	ft_test_line_parsing(t_list **cmds, int sublevel);

// error

void	ft_put_error(char *error, char *extra);
void	ft_error(t_data *d, char *error, char *extra);

// signal

void	ft_signal(t_data *d);
void	ft_signal_sigint(t_data *d);
void	ft_signal_sigquit(t_data *d);
void	ft_signal_child(t_data *d);
void	ft_signal_heredoc(t_data *d);
void	ft_handler_sigint(int signum);
void	ft_handler_sigint_cmd(int signum);
void	ft_handler_sigquit_cmd(int signum);

// line

char	*ft_read_line(t_data *d);
bool	ft_check_line_syntax(char *line);
bool	ft_check_subline_syntax(char *line);
void	ft_parse_line(t_data *d, t_list **cmds, char *line, t_ctrlop type);

// cmd

void	ft_init_cmd(t_cmd_data *cmd);
void	ft_clear_cmds(t_list **cmds);
void	ft_parse_cmd(t_data *d, t_cmd_data *cmd);
char	**ft_get_cmd_args(char *line, char *name);
void	ft_expand_cmd(t_data *d, t_cmd_data *cmd);
void	ft_open_cmd_infile(t_data *d, t_cmd_data *cmd);
void	ft_open_cmd_outfile(t_data *d, t_cmd_data *cmd);
void	ft_heredoc_cmds(t_data *d, t_list **cmds);
void	ft_exec_fork_cmd(t_data *d, t_list *cmd);
void	ft_exec_cmds(t_data *d, t_list **cmds);
void	ft_subshell(t_data *d, t_cmd_data *parent_cmd, t_list **cmds);

/* UTILS ******************************************************************** */

// chr

bool	ft_isop(char c);
bool	ft_isfileop(char c);
bool	ft_isctrlop(char c);
bool	ft_ismeta(char c);
bool	ft_isquote(char c);

// str

char	*ft_str_despace(char *str);
char	*ft_str_expand(t_data *d, char *str);
void	ft_skip_quote(char *str, size_t *i);
void	ft_skip_dquote(char *str, size_t *i);
void	ft_skip_quotes(char *str, size_t *i);
bool	ft_isvar(char *str, size_t i);
bool	ft_ispath_dir(char *path);
bool	ft_ispath_file(char *path);
bool	ft_ispath_exec(char *path);
bool	ft_ispath(char *str);

// wildcard

char	*ft_str_wildcard(char *str);
bool	ft_iswildcard(char *str);
bool	ft_is_valid_file(struct dirent *dirent);
bool	ft_match(char *pattern, char *str);
char	*ft_process_wildcard(char *str, char *pattern);

// env

bool	ft_init_env(t_data *d);
void	ft_clear_env(t_list **env);
char	*ft_get_env(t_data *d, char *key);
char	**ft_get_env_map(t_data *d, bool all);
bool	ft_set_env(t_data *d, char *key, char *value, bool exported);
bool	ft_unset_env(t_data *d, char *key);

// process

bool	ft_exec(t_data *d, t_cmd_data *cmd);
void	ft_exec_builtin(t_data *d, t_cmd_data *cmd);
t_bin	ft_isbuiltin(char *name);
pid_t	ft_pipe_fork(t_data *d, int *fd);
pid_t	ft_fork(t_data *d);
int		ft_waitpid(pid_t pid);

// termios

bool	ft_init_term(t_data *d);
bool	ft_reset_term(t_data *d);

/* COMMANDS ***************************************************************** */

int		ft_cd(t_data *d, char **args);
int		ft_echo(char **args);
int		ft_env(t_data *d);
void	ft_exit(t_data *d, char *msg, int exitcode);
void	ft_exit_cmd(t_data *d, char **args);
int		ft_export(t_data *d, char **args);
int		ft_pwd(t_data *d);
int		ft_unset(t_data *d, char **args);

#endif
