/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sean <sean@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 21:37:48 by seolryeong        #+#    #+#             */
/*   Updated: 2022/03/07 20:24:35 by sean             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_exit(t_data *data, int process)
{
	if (process == 0)
	{
		printf("exit\n");
		tcsetattr(STDIN_FILENO, TCSANOW, &data->term.ori_term);
		exit(0);
	}
}
