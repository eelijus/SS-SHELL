/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sean <sean@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/01 16:23:18 by okwon             #+#    #+#             */
/*   Updated: 2022/03/11 20:37:24 by sean             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	dfree(char **str)
{
	int	i;

	i = -1;
	while (str[++i])
		free(str[i]);
	free(str);
}

void	difree(int **darr)
{
	int	i;

	i = -1;
	while (darr[++i])
		free(darr[i]);
	free(darr);
}

int	dlen(char **darr)
{
	int	i;

	i = 0;
	while (darr[i])
		++i;
	return (i);
}

void	prompt(int flag)
{
	if (flag == 1)
		write(1, "SS-SHELL$ ", 10);
	else if (flag == 2)
	{
		write(1, "\n", 1);
		write(1, "SS-SHELL$ ", 10);
	}
	else if (flag == 3)
	{
		write(1, "SS-SHELL$ ", 10);
		write(1, "\n", 1);
	}
}

void	cexit(char *str)
{
	exit(0);
	printf("%s\n", str);
}
