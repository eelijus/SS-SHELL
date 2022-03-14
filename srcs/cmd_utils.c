/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sean <sean@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 14:02:49 by okwon             #+#    #+#             */
/*   Updated: 2022/03/11 16:48:24 by sean             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	**fds_set(int row)
{
	int	i;
	int	**fds;

	fds = (int **)malloc(sizeof(int *) * (row + 1));
	fds[row] = 0;
	while (--row >= 0)
		fds[row] = (int *)malloc(sizeof(int) * 2);
	return (fds);
}

void	cmd_init(char ***cmd)
{
	int	i;

	*cmd = (char **)malloc(sizeof(char *) * (CMD_NUM + 1));
	i = -1;
	(*cmd)[++i] = ft_strdup(CMD_ECHO);
	(*cmd)[++i] = ft_strdup(CMD_CD);
	(*cmd)[++i] = ft_strdup(CMD_PWD);
	(*cmd)[++i] = ft_strdup(CMD_EXPORT);
	(*cmd)[++i] = ft_strdup(CMD_UNSET);
	(*cmd)[++i] = ft_strdup(CMD_ENV);
	(*cmd)[++i] = ft_strdup(CMD_EXIT);
	(*cmd)[++i] = 0;
}
