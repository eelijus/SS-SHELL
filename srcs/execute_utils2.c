/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sean <sean@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 20:22:04 by sean              #+#    #+#             */
/*   Updated: 2022/03/18 21:46:42 by sean             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exec_cmd_init(t_cmd *cmd, t_pipe *pip)
{
	cmd->redir = 0;
	cmd->file = 0;
	cmd->cmd = 0;
	cmd->pipe_count = pip->pipe_count;
	cmd->cmd_arg = (char **)malloc(sizeof(char *) * 1);
	cmd->cmd_arg[0] = 0;
}

void	exec_init(t_exec *exec, t_cmd *cmd)
{
	int	i;

	exec->env_path = ft_split(ft_getenv("PATH"), ':');
	if (cmd->cmd)
		exec->split_cmd = ft_split(cmd->cmd, ' ');
	else
	{
		exec->split_cmd = (char **)malloc(sizeof(char *) * 1);
		exec->split_cmd[0] = 0;
	}
	// if (!ft_strncmp(exec->split_cmd[0], "cat", ft_strlen(exec->split_cmd[0])))
	// {
	// 	dfree(exec->split_cmd);
	// 	exec->split_cmd = (char **)malloc(sizeof(char *) * 1);
	// 	exec->split_cmd[0] = 0;
	// 	ft_export_split(cmd, &exec->split_cmd);
	// }
	if (!ft_strchr(cmd->cmd, '"' || !ft_strchr(cmd->cmd, '\'')))
	{
		dfree(exec->split_cmd);
		exec->split_cmd = (char **)malloc(sizeof(char *) * 1);
		exec->split_cmd[0] = 0;
		ft_export_split(cmd, &exec->split_cmd);
	}
	if (dlen(cmd->cmd_arg))
	{
		i = -1;
		while (cmd->cmd_arg[++i])
			exec->split_cmd = ft_addonestring(exec->split_cmd, cmd->cmd_arg[i]);
	}
}

int	quote_detec_loop(char *input, char delimeter, int i)
{
	i = i + 1;
	while (input[i] && (input[i] != delimeter))
	{
		i++;
	}
	return (i);
}
