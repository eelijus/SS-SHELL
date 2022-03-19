/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sujilee <sujilee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 16:17:25 by okwon             #+#    #+#             */
/*   Updated: 2022/03/19 16:47:09 by sujilee          ###   ########.fr       */
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
	char	r[256];
	int		fd;

	fd = open("test.txt", O_RDWR, 0644);
	while (read(fd, r, 256) > 0)
		write(STDIN_FILENO, r, ft_strlen(r));
	close(fd);
}

void	do_here(char *cur_cmd, t_cmd *cmd)
{
	char	*input;
	char	r[128];
	int		fd;
	t_data	data;

	fd = open("test.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
	while (1)
	{
		input = input_recive(&data);
		if (ft_strncmp(input, "\n", 1) && \
		 	!ft_strncmp(input, cmd->file, ft_strlen(input) - 1))
			break ;
		write(fd, input, ft_strlen(input));
		if (g_glob.cflag == 1)
		{
			write(1, "\n", 1);
			break ;
		}
	}
	if (g_glob.cflag != 1 && cmd->cmd)
		ft_here_echo();
	close(fd);
	//sujilee free
	free(input);
	fd = unlink("test.txt");
}

void	shell_loop(t_data *data)
{
	char	**cmd_split;
	char	*cmd_line;
	t_cmd	cmd;

	g_glob.cflag = 0;
	data->input.log = 0;
	while (1)
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &data->term.term1);
		determine_prompt();
		cmd_line = input_recive(data);
		make_log(&data->input, cmd_line);
		cmd_split = ft_split(cmd_line, ';');
		cmd_handler(cmd_split, cmd_line, data);
		if (cmd_line)
			free(cmd_line);
		dfree(cmd_split);
	}
	ft_lstclear(&data->input.log, &free);
	free(data->input.log);
}

void	determine_prompt(void)
{
	if (g_glob.cflag == 3)
		prompt(3);
	if (g_glob.cflag == 2)
		prompt(2);
	if (g_glob.cflag == 0)
		prompt(0);
	if (g_glob.cflag == 1)
		prompt(1);
}

void	signal_handler2(int signo)
{
	if (g_glob.cflag == 2)
		g_glob.cflag = 3;
}

int	main(int ac, char **av, char **env)
{
	t_data	data;

	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler2);
	g_glob.envp = ft_sstrdup(env);
	term_init(&data.term.term1, &data.term.ori_term);
	cmd_init(&data.cmd);
	shell_loop(&data);
}
