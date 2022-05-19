#include "make_commands.h"
#define FLAG1 1

void	init_values(t_cmd *cmd)
{
	//src->temp = 1;
	// src->currpos = -1;
	cmd->fd_in = 0;
	cmd->fd_out = 1;
	cmd->args = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->heredoc = NULL;
	cmd->count_args = 0;
	cmd->count_infiles = 0;
	cmd->count_outfiles = 0;
	cmd->count_heredocs = 0;
}



// int	check_each_command(t_cmd *cmd, t_infos *info)
int	check_each_command(t_cmd *cmd)
{
	//printf(GRN" START of check each command: cmd[0]: [%s]\n"RES, cmd->args[0]);

	if (cmd->args == NULL)
		return (1);
	if (strcmp("echo", cmd->args[0]) == 0)
		return (1);
	else if (strcmp("pwd", cmd->args[0]) == 0)
		return (1);
	else if (strcmp("cd", cmd->args[0]) == 0)
		return (1);
	else if (strcmp("export", cmd->args[0]) == 0)
		return (1);
	else if (strcmp("unset", cmd->args[0]) == 0)
		return (1);
	else if (strcmp("env", cmd->args[0]) == 0)
		return (1);
	else if (strcmp("exit", cmd->args[0]) == 0)
		return (1);
	return (0);
}


int	check_if_builtin(t_cmd *cmd)
{
	t_cmd	*temp;
	int		ret;

	// printf(GRN"START of check_if_builtin: list[0]: [%s]\n"RES, list->args[0]);
	temp = cmd;
	//printf(GRN"START of check_if_builtin: temp[0]: [%s]\n"RES, temp->args[0]);
	while (temp)
	{
		//printf(MAG"cmd: [%s]\n"RES, temp->args[0]);
		// ret = check_each_command(temp, info);
		ret = check_each_command(temp);
		//printf(GRN"   ret from check_each_command: %d\n"RES, ret);

		if (ret == 1)
			return (1);
		// else if (ret == 222)	// must exit!
		// 	return (222);
		temp = temp->next;
	}
	return (0);
}

int	exec_builtin(t_cmd *cmd, t_infos *info)
{
	//printf(GRN" exec_builtin: run each command: cmd[0]: [%s], exitcode: %d\n"RES, cmd->args[0], info->exit_code);

	if (cmd->args == NULL)
		return (1);
	if (strcmp("echo", cmd->args[0]) == 0)
		run_echo_builtin(cmd /*, info */);
	else if (strcmp("pwd", cmd->args[0]) == 0)
		run_pwd_builtin();
	else if (strcmp("cd", cmd->args[0]) == 0)
	{
		//printf(GRN"   Found CD\n"RES);
		run_cd_builtin(cmd, info);
	}
	else if (strcmp("exit", cmd->args[0]) == 0)
		run_exit_builtin(cmd /*, info */ /*, list */);

	//printf(GRN" END of check each command\n"RES);
	else if (strcmp("export", cmd->args[0]) == 0)
		g_status = run_export_builtin(cmd, info);
	else if (strcmp("unset", cmd->args[0]) == 0)
		g_status = run_unset_builtin(cmd, info);
	else if (strcmp("env", cmd->args[0]) == 0)
		g_status = run_env_builtin(info);

	//printf(GRN" END of check each command\n"RES);

	return (0);	
}

	// HERE PROBABLY NEEDS TO FREE AND CLEAN ALL, BEFORE EXITING?

void	ft_lstadd_back(t_cmd **list, t_cmd *newnode)
{
	t_cmd *temp;

	if (!*list)
	{
		newnode->next = NULL;
		*list = newnode;
		return ;
	}
	temp = *list;
	while (temp->next)
	{
		//printf("     loop next\n");
		temp = temp->next;
	}
	temp->next = newnode;
	newnode->next = NULL;
}


int	get_length_of_word(t_source *src) //kito changed 12 May : ignore spaces when it is in quotes 
{
	int	len;
	int	s_flag;
	int	d_flag;

	len = 0;
	s_flag = 0;
	d_flag = 0;
	while (src->inputline[src->currpos] != '|' && 
			src->inputline[src->currpos] != '<' &&
			src->inputline[src->currpos] != '>' &&
			src->inputline[src->currpos] != '\0' &&
			!(src->inputline[src->currpos] == ' ' && s_flag + d_flag == 0))
	{
		//printf(LGRE"     len%d pos%ld[%c]\n", len, src->currpos, src->inputline[src->currpos]); // store rdr_in
		if (src->inputline[src->currpos] == '\'' )
		{
			s_flag ^= FLAG1;
			// printf("s_flag is %d\n", (s_flag & FLAG1));
		}			
		if (src->inputline[src->currpos] == '\"' )
		{
			d_flag ^= FLAG1;
			// printf("d_flag is %d\n", (d_flag & FLAG1));
		}		
		len++;
		src->currpos++;
	}
	src->currpos -= 1;
	return (len);
}
