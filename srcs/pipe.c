/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sean <sean@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 16:14:14 by okwon             #+#    #+#             */
/*   Updated: 2022/03/07 13:59:26 by sean             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	c_pipe(int *old_fds, int *fds, int old_flag, int flag)
{
	if (old_flag == '|')
	{
		close(STDIN_FILENO);
		close(old_fds[1]);
		dup2(old_fds[0], 0);
		close(old_fds[0]);
	}
	if (flag == '|')
	{
		close(STDOUT_FILENO);
		close(fds[0]);
		dup2(fds[1], 1);
		close(fds[1]);
	}
}

void	p_pipe(int *old_fds, int *fds)
{
	close(fds[1]);
	close(old_fds[1]);
}
