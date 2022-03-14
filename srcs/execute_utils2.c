/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sean <sean@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 20:22:04 by sean              #+#    #+#             */
/*   Updated: 2022/03/11 16:41:48 by sean             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exec_cmd_init(t_cmd *cmd, t_pipe *pip)
{
	cmd->redir = ft_strdup("");
	cmd->file = ft_strdup("");
	cmd->cmd = ft_strdup("");
	cmd->pipe_count = pip->pipe_count;
}

void	exec_init(t_exec *exec, t_cmd *cmd)
{
	exec->env_path = ft_split(getenv("PATH"), ':');
	exec->split_cmd = ft_split(cmd->cmd, ' ');
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
