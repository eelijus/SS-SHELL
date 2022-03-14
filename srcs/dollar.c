/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sean <sean@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 17:15:31 by sean              #+#    #+#             */
/*   Updated: 2022/03/11 20:21:59 by sujilee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_glob	g_glob;

char	*ft_getenv(char *env)
{
	int		i;
	char	**split;
	char	*ret;

	i = -1;
	if (!ft_strncmp(env, "$", ft_strlen(env)))
		return (ft_strdup("$"));
	if (!ft_strncmp(env, "?", ft_strlen(env)))
		return (ft_strdup(ft_itoa(g_glob.exit_status)));
	while ((g_glob.envp)[++i])
	{
		split = ft_split((g_glob.envp)[i], '=');
		if (!ft_strncmp(split[0], env, ft_strlen(env)))
		{
			ret = ft_strdup(split[1]);
			dfree (split);
			return (ret);
		}
		dfree(split);
	}
	return (ft_strdup(""));
}

char	*dollar(int i, char *input, t_cmd *cmd)
{
	int		len;
	char	*tmp;
	char	*get_cmd;

	len = 0;
	while (input[++i] && input[i] != ' ')
		len++;
	if (len == 0)
		get_cmd = ft_getenv(input);
	else
	{
		tmp = ft_substr(input, i - len, len);
		get_cmd = ft_getenv(tmp);
	}
	free(tmp);
	if (get_cmd != NULL)
		return (get_cmd);
	else
		return (input);
}
