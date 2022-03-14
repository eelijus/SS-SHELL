/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sean <sean@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 21:17:39 by sean              #+#    #+#             */
/*   Updated: 2022/03/11 20:25:37 by sujilee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	builtin_cmd_check(t_data *data, char *cmd)
{
	int	i;

	i = -1;
	while (++i < CMD_NUM)
		if (!ft_strncmp(cmd, data->cmd[i], ft_strlen(cmd)) && \
		!ft_strncmp(cmd, data->cmd[i], ft_strlen(data->cmd[i])))
			return (1);
	return (0);
}

void	redirection_in(t_cmd *cmd)
{
	struct stat	status;
	int			fd;

	if (stat(cmd->cur_file, &status) == -1)
		print_error("no such file or directory\n", 1, cmd);
	fd = open(cmd->cur_file, O_RDONLY);
	dup2(fd, 0);
	close(fd);
}

void	redirection_out(t_cmd *cmd)
{
	int	fd;

	if (!ft_strncmp(cmd->redir, ">", ft_strlen(cmd->redir)))
		fd = open(cmd->cur_file, O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (!ft_strncmp(cmd->redir, ">>", ft_strlen(cmd->redir)))
		fd = open(cmd->cur_file, O_RDWR | O_CREAT | O_APPEND, 0644);
	else
		print_error("syntax error near unexpected token\n", 2, cmd);
	dup2(fd, 1);
	close(fd);
}

int	redirection_check2(t_cmd *cmd, char *input, int i)
{
	t_cmd	tmp;
	int		ret;
	char	*tmp2;

	ret = make_token(&tmp, input);
	cmd->cur_file = handle_quotes(tmp.cmd, cmd);
	cmd->next_redir = tmp.redir;
	return (ret);
}

void	redirection_check(t_cmd *cmd)
{
	int	fd;
	int	i;

	i = -1;
	if (cmd->file)
	{
		while ((cmd->file)[++i])
		{
			i += redirection_check2(cmd, &(cmd->file)[i], i);
			if (!ft_strncmp(cmd->redir, "<", ft_strlen(cmd->redir)))
				redirection_in(cmd);
			else
				redirection_out(cmd);
			cmd->redir = cmd->next_redir;
		}
	}
}
