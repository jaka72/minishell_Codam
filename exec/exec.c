#include "../minishell.h"

void	init_pid_sig(t_pid *pidinfo)
{
	pidinfo->status = 0;
	pidinfo->newpipe[0] = 0;
	pidinfo->newpipe[1] = 0;
	pidinfo->newpipe[2] = 0;
	pidinfo->temp_cmd = g_gl.start_cmd;
	signal(SIGINT, handle_sigint_p);
	signal(SIGQUIT, handle_sigquit_p);
}

static int	exec_no_pipe(t_pid *pid)
{
	if (check_file_access(g_gl.start_cmd) != 0)
		return (1);
	if (g_gl.start_cmd->args == NULL)
		return (0);
	g_gl.start_cmd->args = expand_array(g_gl.start_cmd->args);
	if (connect_fd(g_gl.start_cmd) != 0)
		g_gl.g_status = 1;
	else if (check_if_builtin(g_gl.start_cmd) == 1)
		g_gl.g_status = exec_builtin(g_gl.start_cmd, g_gl.start_cmd);
	else
	{
		pid->cu_pid = fork();
		if (pid->cu_pid == 0)
			ms_execve(g_gl.start_cmd);
		check_close_fd(g_gl.start_cmd->fd_in, g_gl.start_cmd->fd_out);
		waitpid(pid->cu_pid, &g_gl.g_status, WUNTRACED | WCONTINUED);
		if (WIFEXITED(g_gl.g_status))
			g_gl.g_status = WEXITSTATUS(g_gl.g_status);
		else if (WIFEXITED(g_gl.g_status) == 0 && WIFSIGNALED(g_gl.g_status))
			g_gl.g_status = 128 + WTERMSIG(g_gl.g_status);
	}
	reset_fd_sig();
	return (g_gl.g_status);
}

static void	connect_fd_child(t_cmd	*current, t_pid *pid)
{
	if (current != g_gl.start_cmd)
		g_gl.g_status = 0;
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
	if (check_file_access(current) != 0)
		exit(err_all_free_exit(1));
	if (connect_fd(current) != 0)
		exit(err_all_free_exit(1));
	if (current->args == NULL)
		exit(0);
	current->args = expand_array(current->args);
	if (check_if_builtin(current) == 1)
		exit(exec_builtin(current, g_gl.start_cmd));
}

static int	exec_with_pipe(t_pid *pid)
{
	pid->temp_cmd = g_gl.start_cmd;
	while (pid->temp_cmd)
	{
		if (pid->temp_cmd != g_gl.start_cmd)
			g_gl.g_status = 0;
		if (pid->temp_cmd->next)
			pipe(pid->newpipe);
		pid->cu_pid = fork();
		if (pid->cu_pid == 0)
		{
			connect_fd_child(pid->temp_cmd, pid);
			ms_execve(pid->temp_cmd);
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

int	run_cmd(void)
{
	t_cmd	*current;
	t_pid	pid;

	init_pid_sig(&pid);
	if (open_heredoc(&pid) != 0)
		return (g_gl.g_status);
	if (g_gl.start_cmd->next == NULL)
		return (exec_no_pipe(&pid));
	exec_with_pipe(&pid);
	current = g_gl.start_cmd;
	while (current)
	{
		if (waitpid(0, &pid.status, WUNTRACED | WCONTINUED) == pid.last_pid)
		{
			if (WIFEXITED(pid.status))
				g_gl.g_status = WEXITSTATUS(pid.status);
			else if (WIFEXITED(pid.status) == 0 && WIFSIGNALED(pid.status))
				g_gl.g_status = 128 + WTERMSIG(pid.status);
		}
		current = current->next;
	}
	reset_fd_sig();
	return (g_gl.g_status);
}
