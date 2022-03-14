/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sujilee <sujilee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 16:21:58 by okwon             #+#    #+#             */
/*   Updated: 2022/03/11 20:50:02 by sujilee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_glob	g_glob;

int	choose_process(int size, char *cmd)
{
	char	**split;
	int		ret;

	if (size == 1)
	{
		split = ft_split(cmd, ' ');
		if (!split[0])
			ret = 0;
		if (!ft_strncmp(split[0], "cd", ft_strlen(split[0])))
			ret = 0;
		else if (!ft_strncmp(split[0], "exit", ft_strlen(split[0])))
			ret = 0;
		else if (!ft_strncmp(split[0], "export", ft_strlen(split[0])))
			ret = 0;
		else if (!ft_strncmp(split[0], "unset", ft_strlen(split[0])))
			ret = 0;
		dfree(split);
		return (ret);
	}
	return (1);
}

void	execute(t_data *data, t_pipe *pip, int row)
{
	pid_t	pid;
	int		status;
	char	**split;

	pipe(pip->fds);
	pid = fork();
	if (pid == 0)
	{
		c_pipe(pip->old_fds, pip->fds, pip->old_flag, pip->flag);
		if (choose_process(row, pip->cur_cmd))
			execute_input(data, pip, 1);
		exit(g_glob.exit_status);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		g_glob.exit_status = WEXITSTATUS(status);
		if (choose_process(row, pip->cur_cmd) == 0)
			execute_input(data, pip, 0);
		p_pipe(pip->old_fds, pip->fds);
	}
}

void	p_func(char **cmd_split, char *cmd_line, int **fds, t_data *data)
{
	t_pipe	pip;
	int		stack;
	int		i;

	pip.old_flag = 0;
	stack = 0;
	i = -1;
	pip.pipe_count = data->pipecmd_len;
	while (++i < data->pipecmd_len)
	{
		stack += ft_strlen(cmd_split[i]);
		while (cmd_line[stack] == ' ')
			++stack;
		pip.cur_cmd = cmd_split[i];
		if (i > 0)
			pip.old_fds = fds[i - 1];
		else
			pip.old_fds = fds[0];
		pip.fds = fds[i];
		if (stack < ft_strlen(cmd_line))
			pip.flag = cmd_line[stack];
		else
			pip.flag = 0;
		execute(data, &pip, data->pipecmd_len);
		pip.old_flag = pip.flag;
	}
}

char	**ft_pipe_split(char *cmd, char delimeter)
{
	int		start;
	int		i;
	char	**split_cmd;
	char	*add_line;

	split_cmd = (char **)malloc(sizeof(char *) * 1);
	split_cmd[0] = NULL;
	start = 0;
	i = -1;
	while (cmd[++i])
	{
		if (cmd[i] == '\'' || cmd[i] == '"')
			i = quote_detec_loop(cmd, cmd[i], i);
		if (cmd[i] == delimeter)
		{
			add_line = ft_substr(cmd, start, i);
			split_cmd = ft_addonestring(split_cmd, add_line);
			start = i + 1;
			free(add_line);
		}
	}
	if (start != i)
	{
		add_line = ft_substr(cmd, start, i);
		split_cmd = ft_addonestring(split_cmd, add_line);
		free(add_line);
	}
	return (split_cmd);
}

void	cmd_handler(char **cmd_split, char *cmd_line, t_data *data)
{
	int		**fds;
	pid_t	pid;
	int		status;
	int		i;
	char	**cmd_pipe;

	i = -1;
	while (++i < dlen(cmd_split))
	{
		cmd_pipe = ft_pipe_split(cmd_split[i], '|');
		data->pipecmd_len = dlen(cmd_pipe);
		pid = fork();
		fds = fds_set(dlen(cmd_pipe));
		if (pid == 0)
		{
			if (choose_process(dlen(cmd_pipe), cmd_pipe[0]))
				p_func(cmd_pipe, cmd_split[i], fds, data);
			exit(g_glob.exit_status);
		}
		else if (pid > 0)
		{
			waitpid(pid, &status, 0);
			g_glob.exit_status = WEXITSTATUS(status);
			if (choose_process(dlen(cmd_pipe), cmd_pipe[0]) == 0)
				p_func(cmd_pipe, cmd_split[i], fds, data);
			difree(fds);
		}
		dfree(cmd_pipe);
	}
}
