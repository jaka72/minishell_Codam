/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   typedef.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/22 12:16:45 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPEDEF_H
# define TYPEDEF_H

typedef struct s_global		t_global;
typedef struct s_env		t_env;
typedef struct s_cmd		t_cmd;
typedef struct s_source		t_source;
typedef struct s_pid		t_pid;
extern t_global				g_gl;

struct	s_global
{
	int				g_status;
	t_env			*start_env;
	t_cmd			*start_cmd;
	char			prompt[13];
	int				ini_fd[2];
	struct termios	termios_save;
};

struct s_env
{
	char	*name;
	char	*value;
	t_env	*next;
};

struct s_cmd
{
	char			**args;
	int				fd_in;
	int				fd_out;
	char			**heredoc;
	char			**files;
	struct s_cmd	*next;
};

struct s_source
{
	char	*inputline;
	long	inputline_size;
	long	currpos;
};

struct s_pid
{
	pid_t	cu_pid;
	int		newpipe[3];
	int		status;
	pid_t	last_pid;
	t_cmd	*temp_cmd;
};

typedef struct s_temporary_array
{
	int		arrow;
	int		len;
	int		start;
	char	**temp_arr;
	int		count;
}	t_tmp;

#endif
