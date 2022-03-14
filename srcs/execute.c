/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sean <sean@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 17:09:12 by sean              #+#    #+#             */
/*   Updated: 2022/03/11 20:21:30 by sujilee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_glob	g_glob;

void	print_error(char *message, int code, t_cmd *cmd)
{
	write(cmd->std_out, message, ft_strlen(message));
	if (!choose_process(cmd->pipe_count, cmd->cmd))
	{
		g_glob.exit_status = code;
	}
	else
		exit(code);
}

void	do_execute(t_cmd *cmd, t_exec *exec, t_data *data, int process)
{
	char	*split_cmd;

	split_cmd = exec->split_cmd[0];
	if (!ft_strncmp(split_cmd, "echo", ft_strlen(split_cmd)))
		ft_echo(cmd, *exec, data);
	else if (!ft_strncmp(split_cmd, "pwd", ft_strlen(split_cmd)))
		ft_pwd(cmd, *exec, data);
	else if (!ft_strncmp(split_cmd, "cd", ft_strlen(split_cmd)))
		ft_cd(cmd, exec->split_cmd[1], process);
	else if (!ft_strncmp(split_cmd, "env", ft_strlen(split_cmd)))
		ft_env(data);
	else if (!ft_strncmp(split_cmd, "exit", ft_strlen(split_cmd)))
		ft_exit(data, process);
	else if (!ft_strncmp(split_cmd, "unset", ft_strlen(split_cmd)))
		ft_unset(cmd, data);
	else if (!ft_strncmp(split_cmd, "export", ft_strlen(split_cmd)))
		ft_export(cmd);
}

void	execute_cmd(t_data *data, t_cmd *cmd, int process)
{
	t_exec	exec;
	int		flag;

	flag = 0;
	exec_init(&exec, cmd);
	if (builtin_cmd_check(data, exec.split_cmd[0]))
	{
		do_execute(cmd, &exec, data, process);
		if (g_glob.exit_status == -1)
			g_glob.exit_status = 1;
		else
			g_glob.exit_status = 0;
		dfree(exec.env_path);
		dfree(exec.split_cmd);
		free(exec.path);
	}
	else
	{
		if (make_path(&exec))
			print_error("command not found\n", 127, cmd);
		else
			execve(exec.path, get_argv(exec.split_cmd, exec.path), g_glob.envp);
	}
}

void	execute_input(t_data *data, t_pipe *pipe, int process)
{
	t_cmd	cmd;
	char	*tmp;

	cmd.std_in = dup(0);
	cmd.std_out = dup(1);
	exec_cmd_init(&cmd, pipe);
	if (!ft_strncmp(pipe->cur_cmd, "\n", 1))
		exit(0);
	make_token(&cmd, pipe->cur_cmd);
	if (cmd.redir)
		redirection_check(&cmd);
	tmp = handle_quotes(cmd.cmd, &cmd);
	if (cmd.cmd)
		free(cmd.cmd);
	cmd.cmd = tmp;
	execute_cmd(data, &cmd, process);
	if (cmd.cmd)
		free(cmd.cmd);
	if (cmd.redir)
		free(cmd.redir);
	if (cmd.file)
		free(cmd.file);
}
