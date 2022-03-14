/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sujilee <sujilee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 14:43:02 by sujilee           #+#    #+#             */
/*   Updated: 2022/03/12 10:50:25 by sujilee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_sstrlen(char **strstr)
{
	int		len;

	len = 0;
	if (!strstr)
		return (0);
	while (strstr[len] != NULL)
		++len;
	return (len);
}

char	**ft_sstrdup(char **origin)
{
	int		i;
	int		env_len;
	char	**new_env;

	if (!origin)
		return (NULL);
	env_len = ft_sstrlen(origin);
	new_env = (char **)malloc(sizeof(char *) * (env_len + 1));
	i = -1;
	while (++i < env_len)
		new_env[i] = ft_strdup(origin[i]);
	new_env[i] = NULL;
	return (new_env);
}

char	**ft_addonestring(char **origin, char *addline)
{
	char	**new;
	int		idxlen;
	int		i;

	idxlen = 0;
	while (origin[idxlen])
		idxlen++;
	new = (char **)malloc(sizeof(char *) * (idxlen + 2));
	i = -1;
	while (++i < idxlen)
		new[i] = ft_strdup(origin[i]);
	new[idxlen] = ft_strdup(addline);
	new[idxlen + 1] = NULL;
	idxlen = 0;
	dfree(origin);
	origin = 0;
	return (new);
}

void	free_strings(char **str1, char **str2, char **str3, char **str4)
{
	free(*str1);
	free(*str2);
	free(*str3);
	free(*str4);
}
