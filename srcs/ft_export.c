/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sean <sean@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 22:50:55 by seolryeong        #+#    #+#             */
/*   Updated: 2022/03/11 19:38:37 by sean             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_glob	g_glob;

int	check_alnum_newkey(t_cmd *cmd, char *new, char **new_key, int *new_len)
{
	int		i;

	*new_key = ft_substr(new, 0, ft_strchr(new, '=') - new);
	*new_len = ft_strlen(*new_key);
	if (*new_len == 0)
	{
		free(*new_key);
		print_error("minishell: export: \'", -1, cmd);
		print_error(new, -1, cmd);
		print_error("\': not a valid identifier\n", -1, cmd);
		return (1);
	}
	i = -1;
	while (++i < *new_len)
	{
		if (!(ft_isalnum((*new_key)[i]) || (*new_key)[i] == '_'))
		{
			free(*new_key);
			print_error("minishell: export: \'", -1, cmd);
			print_error(new, -1, cmd);
			print_error("\': not a valid identifier\n", -1, cmd);
			return (1);
		}
	}
	return (0);
}

int	deal_export(t_cmd *cmd, char *new)
{
	int		i;
	int		new_len;
	char	*new_key;

	if (check_alnum_newkey(cmd, new, &new_key, &new_len) == 1)
		return (1);
	i = -1;
	while (++i < (int)ft_sstrlen(g_glob.envp))
	{
		if (!ft_strncmp(g_glob.envp[i], new_key, new_len)
			&& (new[new_len] == '\0' || new[new_len] == '='))
		{
			if (new[new_len] == '=')
			{
				free(g_glob.envp[i]);
				g_glob.envp[i] = ft_strdup(new);
			}
			free(new_key);
			return (0);
		}
	}
	free(new_key);
	g_glob.envp = ft_addonestring(g_glob.envp, new);
	return (0);
}

void	print_sorted_env(char **sorted_env)
{
	char	**tmpsstr;
	int		idx;

	idx = 0;
	while (sorted_env[idx] != NULL)
	{
		tmpsstr = ft_split(sorted_env[idx], '=');
		printf("declare -x %s", tmpsstr[0]);
		if (tmpsstr[1])
			printf("=\"%s\"", tmpsstr[1]);
		else if (sorted_env[idx][ft_strlen(sorted_env[idx]) - 1] == '=')
			printf("=\"\"");
		printf("\n");
		dfree(tmpsstr);
		idx++;
	}
	dfree(sorted_env);
}

void	b_export_noarg(void)
{
	int		i;
	int		j;
	char	**sorted_env;
	char	*tmp;

	sorted_env = ft_sstrdup(g_glob.envp);
	i = -1;
	while (++i < (int)ft_sstrlen(sorted_env) - 1)
	{
		j = 0;
		while (i + j < (int)ft_sstrlen(sorted_env) - 1)
		{
			if (ft_strncmp(sorted_env[j], sorted_env[j + 1],
					ft_strlen(sorted_env[j])) > 0)
			{
				tmp = sorted_env[j];
				sorted_env[j] = sorted_env[j + 1];
				sorted_env[j + 1] = tmp;
			}
			j++;
		}
	}
	print_sorted_env(sorted_env);
}

void	ft_export(t_cmd *cmd)
{
	int		i;
	char	**split;

	split = ft_split(cmd->cmd, ' ');
	if (split[1] == NULL)
		b_export_noarg();
	i = 1;
	while (split[i])
	{
		if (deal_export(cmd, split[i]) == 1)
			break ;
		i++;
	}
	dfree(split);
}
