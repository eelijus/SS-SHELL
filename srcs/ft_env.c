/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sujilee <sujilee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 21:20:36 by seolryeong        #+#    #+#             */
/*   Updated: 2022/03/07 16:12:07 by sujilee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_glob	g_glob;

void	ft_env(t_data *data)
{
	char	**split;
	int		i;

	i = -1;
	while ((g_glob.envp)[++i])
		printf("%s\n", (g_glob.envp)[i]);
}
