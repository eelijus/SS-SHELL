/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sujilee <sujilee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 16:21:58 by okwon             #+#    #+#             */
/*   Updated: 2022/03/19 16:17:37 by sujilee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_glob	g_glob;

int	choose_process(int size, char *cmd)
{
	char	**split;

	if (size == 1)
	{
		split = ft_split(cmd, ' ');
		if (!split[0])
			return (1);
		if (!ft_strncmp(split[0], "cd", ft_strlen(split[0])))
			return (0);
		else if (!ft_strncmp(split[0], "exit", ft_strlen(split[0])))
			return (0);
		else if (!ft_strncmp(split[0], "export", ft_strlen(split[0])))
			return (0);
		else if (!ft_strncmp(split[0], "unset", ft_strlen(split[0])))
			return (0);
		dfree(split);
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
		c_pipe(pip);
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

char	**ft_pipe_split(char *cmd)
{
	int		start;
	int		i;
	char	**split_cmd;
	char	*add_line;
	t_string	str;

	split_cmd = (char **)malloc(sizeof(char *) * 1);
	split_cmd[0] = NULL;
	start = 0;
	i = -1;
	while (cmd[++i])
	{
		if (cmd[i] == '\'' || cmd[i] == '"')
			i = quote_detec_loop(cmd, cmd[i], i);
		if (cmd[i] == '|')
		{
			//sujilee free
			str.one = ft_substr(cmd, start, i - start);
			split_cmd = ft_addonestring(split_cmd, str.one);
			start = i + 1;
			free(str.one);
		}
	}
	if (start != i)
	{
		//sujilee free
		str.two = ft_substr(cmd, start, i);
		split_cmd = ft_addonestring(split_cmd, str.two);
		free(str.two);
	}
	return (split_cmd);
}

void	set_pipe_fds(int i, int stack, t_pipe *pip, int **fds)
{
	if (i > 0)
		pip->old_fds = fds[i - 1];
	else
		pip->old_fds = fds[0];
	pip->fds = fds[i];
	if (stack < ft_strlen(pip->ori_cmd_line))
		pip->flag = pip->ori_cmd_line[stack];
	else
		pip->flag = 0;
}

void	p_func(char **cmd_split, char *cmd_line, int **fds, t_data *data)
{
	t_pipe	pip;
	int		stack;
	int		i;

	pip.old_flag = 0;
	stack = 0;
	i = -1;
	pip.pipe_count = dlen(cmd_split);
	pip.ori_cmd_line = cmd_line;
	while (++i < pip.pipe_count)
	{
		stack += ft_strlen(cmd_split[i]);
		while (cmd_line[stack] == ' ')
			++stack;
		pip.cur_cmd = cmd_split[i];
		set_pipe_fds(i, stack, &pip, fds);
		execute(data, &pip, pip.pipe_count);
		pip.old_flag = pip.flag;
	}
}

void	handle_process(char *cmd_split, int **fds, t_data *data)
{
	pid_t	pid;
	int		status;
	char	**cmd_pipe;

	cmd_pipe = ft_pipe_split(cmd_split);
	pid = fork();
	fds = fds_set(dlen(cmd_pipe));
	if (pid == 0)
	{
		if (choose_process(dlen(cmd_pipe), cmd_pipe[0]) == 1)
			p_func(cmd_pipe, cmd_split, fds, data);
		exit(g_glob.exit_status);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		g_glob.exit_status = WEXITSTATUS(status);
		if (choose_process(dlen(cmd_pipe), cmd_pipe[0]) == 0)
			p_func(cmd_pipe, cmd_split, fds, data);
		difree(fds);
	}
	dfree(cmd_pipe);
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
		handle_process(cmd_split[i], fds, data);
	}
}
