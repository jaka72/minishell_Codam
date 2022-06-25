/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: J&K(Jaka and Kito)                           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/06/22 12:10:44 by kito          #+#    #+#                 */
/*   Updated: 2022/06/23 19:27:45 by kito          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_pid_sig(t_pid *pidinfo, t_util *st_base)
{
	pidinfo->status = 0;
	pidinfo->newpipe[0] = 0;
	pidinfo->newpipe[1] = 0;
	pidinfo->newpipe[2] = 0;
	pidinfo->temp_cmd = st_base->start_cmd;
	signal(SIGINT, handle_sigint_p);
	signal(SIGQUIT, handle_sigquit_p);
}

static int	exec_no_pipe(t_pid *pid, int *stat, t_util *base)
{
	if (check_file_access(base->start_cmd, stat, base) != 0)
		return (1);
	if (base->start_cmd->args == NULL)
		return (0);
	base->start_cmd->args = expand_array(base->start_cmd->args, stat, base);
	if (connect_fd(base->start_cmd) != 0)
		*stat = 1;
	else if (check_if_builtin(base->start_cmd) == 1)
		*stat = exec_builtin(base->start_cmd, base->start_cmd, base);
	else
	{
		pid->cu_pid = fork();
		if (pid->cu_pid < 0)
			return (reset_fd_sig(base));
		if (pid->cu_pid == 0)
			ms_execve(base->start_cmd, base);
		check_close_fd(base->start_cmd->fd_in, base->start_cmd->fd_out);
		waitpid(pid->cu_pid, stat, WUNTRACED | WCONTINUED);
		if (WIFEXITED(*stat))
			*stat = WEXITSTATUS(*stat);
		else if (WIFEXITED(*stat) == 0 && WIFSIGNALED(*stat))
			*stat = 128 + WTERMSIG(*stat);
	}
	reset_fd_sig(base);
	return (*stat);
}

static void	connect_fd_c(t_cmd	*cur, t_pid *pid, int *stat, t_util *st_base)
{
	if (cur != st_base->start_cmd)
		*stat = 0;
	if (pid->newpipe[2] != 0)
	{
		dup2(pid->newpipe[2], 0);
		close(pid->newpipe[2]);
	}
	if (pid->newpipe[1] != 0)
	{
		dup2(pid->newpipe[1], 1);
		close(pid->newpipe[1]);
	}
	if (pid->newpipe[0] != 0)
		close(pid->newpipe[0]);
	if (check_file_access(cur, stat, st_base) != 0)
		msg_and_exit("minishell: file access check failed\n", 1);
	if (connect_fd(cur) != 0)
		msg_and_exit("minishell: connecting fd\n", 1);
	if (cur->args == NULL)
		exit(0);
	cur->args = expand_array(cur->args, stat, st_base);
	if (check_if_builtin(cur) == 1)
		exit(exec_builtin(cur, st_base->start_cmd, st_base));
}

static int	exec_with_pipe(t_pid *pid, int *ex_stat, t_util *st_base)
{
	pid->temp_cmd = st_base->start_cmd;
	while (pid->temp_cmd)
	{
		if (pid->temp_cmd != st_base->start_cmd)
			*ex_stat = 0;
		if (pid->temp_cmd->next)
			pipe(pid->newpipe);
		pid->cu_pid = fork();
		if (pid->cu_pid < 0)
			return (wait_return());
		if (pid->cu_pid == 0)
		{
			connect_fd_c(pid->temp_cmd, pid, ex_stat, st_base);
			ms_execve(pid->temp_cmd, st_base);
		}
		if (pid->newpipe[2] != 0)
			close(pid->newpipe[2]);
		pid->newpipe[2] = pid->newpipe[0];
		if (pid->newpipe[1] != 0)
			close(pid->newpipe[1]);
		if (pid->temp_cmd->next == NULL)
			pid->last_pid = pid->cu_pid;
		pid->temp_cmd = pid->temp_cmd->next;
	}
	return (0);
}

int	run_cmd(int *ex_stat, t_util *st_base)
{
	t_cmd	*current;
	t_pid	pid;

	init_pid_sig(&pid, st_base);
	if (open_heredoc(&pid, ex_stat, st_base) != 0)
		return (*ex_stat);
	if (st_base->start_cmd->next == NULL)
		return (exec_no_pipe(&pid, ex_stat, st_base));
	if (exec_with_pipe(&pid, ex_stat, st_base) != 0)
		return (reset_fd_sig(st_base));
	current = st_base->start_cmd;
	while (current)
	{
		if (waitpid(0, &pid.status, WUNTRACED | WCONTINUED) == pid.last_pid)
		{
			if (WIFEXITED(pid.status))
				*ex_stat = WEXITSTATUS(pid.status);
			else if (WIFEXITED(pid.status) == 0 && WIFSIGNALED(pid.status))
				*ex_stat = 128 + WTERMSIG(pid.status);
		}
		current = current->next;
	}
	reset_fd_sig(st_base);
	return (*ex_stat);
}
