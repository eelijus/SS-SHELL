/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 20:22:04 by sean              #+#    #+#             */
/*   Updated: 2022/03/14 11:19:26 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	make_path(t_exec *exec)
{
	int			i;
	char		*tmp;
	struct stat	status;

	i = -1;
	if (ft_strncmp(exec->split_cmd[0], "/bin", 4) == 0)
	{
		exec->path = ft_strdup(exec->split_cmd[0]);
		if (stat(exec->path, &status) == 0)
			return (0);
	}
	while ((exec->env_path)[++i])
	{
		tmp = ft_strjoin((exec->env_path)[i], "/");
		exec->path = ft_strjoin(tmp, (exec->split_cmd)[0]);
		if (stat(exec->path, &status) == 0)
			return (0);
		free(exec->path);
		free(tmp);
	}
	return (1);
}

int	make_token(t_cmd *cmd, char *input)
{
	int		i;
	int		j;
	char	delimeter;
	char	*t1;
	char	*t2;
	char	*t3;

	i = -1;
	j = 0;
	while (input[++i])
	{
		if (input[i] == '"')
			i = quote_detec_loop(input, '"', i);
		if (input[i] == '\'')
			i = quote_detec_loop(input, '\'', i);
		while (ft_strchr("><", input[i]))
		{
			i++;
			j++;
		}
		if (j != 0)
			break ;
	}
	t1 = ft_substr(input, 0, (i - j));
	if (ft_strncmp(t1, "", 1))
	{
		free(cmd->cmd);
		cmd->cmd = ft_strtrim(t1, " ");
		free(t1);
	}
	if (j != 0)
	{
		free(cmd->redir);
		t2 = ft_substr(input, (i - j), j);
		cmd->redir = ft_strtrim(t2, " ");
		free(t2);
	}
	t3 = ft_substr(input, i, ft_strlen(input) - i);
	if (ft_strncmp(t3, "", 1))
	{
		free(cmd->file);
		cmd->file = ft_strtrim(t3, " ");
		free(t3);
	}
	return (i);
}

char	**get_argv(char **split, char *file)
{
	char	**ret;
	int		l;
	int		i;

	l = 0;
	while (split[l])
		++l;
	ret = (char **)malloc(sizeof(char *) * (l + 1));
	i = 0;
	ret[i] = file;
	while (split[++i])
		ret[i] = split[i];
	ret[i] = 0;
	return (ret);
}
