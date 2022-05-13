#include "make_commands.h"


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



int	check_each_command(t_cmd *cmd, t_infos *info)
{
	cmd->temp = 1; // just for testing

	if (strcmp("echo", cmd->args[0]) == 0)
	{
		run_echo_builtin(cmd, info);
	}
	else if (strcmp("pwd", cmd->args[0]) == 0)
	{
		if (cmd->temp == 1) // just for testing
			run_pwd_builtin();
	}
	else if (strcmp("cd", cmd->args[0]) == 0)
	{
		if (cmd->temp == 1) // just for testing
			run_cd_builtin(cmd, info);
	}
	else if (strcmp("export", cmd->args[0]) == 0)
	{
		; //run_export_builtin(cmd);
	}
	//printf("from check each command\n");

	return (0);
}


int	check_if_builtin(t_cmd *cmd, t_infos *info)
{
	t_cmd *temp;

	temp = cmd;
	while (temp)
	{
		//printf(MAG"cmd: [%s]\n"RES, temp->args[0]);
		check_each_command(temp, info);
		temp = temp->next;
	}
	return (0);
}

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


int	get_length_of_word(t_source *src)
{
	int	len;

	len = 0;
	while (src->inputline[src->currpos] != '|' && 
			src->inputline[src->currpos] != '<' &&
			src->inputline[src->currpos] != '>' &&
			src->inputline[src->currpos] != '\0' &&
			!ft_isspace(src->inputline[src->currpos]))
	{
		//printf(LGRE"     len%d pos%ld[%c]\n", len, src->currpos, src->inputline[src->currpos]); // store rdr_in
		len++;
		src->currpos++;
	}
	src->currpos -= 1;
	return (len);
}
