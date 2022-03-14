/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sean <sean@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 16:17:25 by okwon             #+#    #+#             */
/*   Updated: 2022/03/11 20:53:09 by sean             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_glob	g_glob;

void	make_log(t_input *input, char *cmd_line)
{
	if (!ft_strncmp(cmd_line, "\n", ft_strlen(cmd_line)))
		return ;
	cmd_line[ft_strlen(cmd_line) - 1] = '\0';
	ft_lstadd_front(&input->log, ft_lstnew(0, ft_strdup(cmd_line)));
}

void	ft_here_echo(void)
{
	char	r[1024];
	int		fd;
	int		read_size;

	fd = open("test.txt", O_RDWR, 0644);
	while ((read_size = read(fd, r, 1024)) > 0)
	{
		r[read_size] = '\0';
		printf("%s", r);
	}
	close(fd);
}

void	do_here(char *tmp)
{
	char	*re;
	char	**split;
	char	r[128];
	int		fd;
	t_data	data;

	split = ft_split(tmp, ' ');
	fd = open("test.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
	write(fd, "cat << ", 7);
	write(fd, split[2], ft_strlen(split[2]));
	while (1)
	{
		re = input_recive(&data);
		write(fd, re, ft_strlen(re));
		if (!ft_strncmp(re, split[2], ft_strlen(re)))
		{
			free(re);
			break ;
		}
		free(re);
	}
	ft_here_echo();
	dfree(split);
	close(fd);
	fd = unlink("test.txt");
}

void	shell_loop(t_data *data)
{
	char	**cmd_split;
	char	*cmd_line;
	t_cmd	cmd;

	data->input.cflag = 0;
	data->input.log = 0;
	while (1)
	{
		if (data->input.cflag == 0)
			prompt(1);
		cmd_line = input_recive(data);
		if (!ft_strncmp(cmd_line, "cat <<", 6))
		{
			do_here(cmd_line);
			free(cmd_line);
			continue ;
		}
		make_log(&data->input, cmd_line);
		cmd_split = ft_pipe_split(cmd_line, ';');
		cmd_handler(cmd_split, cmd_line, data);
		if (cmd_line)
			free(cmd_line);
		dfree(cmd_split);
	}
	ft_lstclear(&data->input.log, &free);
	free(data->input.log);
}

int	main(int ac, char **av, char **env)
{
	t_data	data;

	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	g_glob.envp = ft_sstrdup(env);
	term_init(&data.term.term1, &data.term.ori_term);
	cmd_init(&data.cmd);
	shell_loop(&data);
}
