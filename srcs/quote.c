/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sujilee <sujilee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 14:21:45 by sean              #+#    #+#             */
/*   Updated: 2022/03/12 10:50:45 by sujilee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_glob	g_glob;

int	not_quotes(char **new_input, char *input, int i, t_cmd *cmd)
{
	int			j;
	int			len;
	char		*tmp;
	t_string	string;

	len = 1;
	j = i;
	while (input[++i] && input[i] != ' ' && input[i] != '"' && \
	input[i] != '\'' && input[i] != '$' && input[i] != '=')
		len++;
	string.tmp2 = ft_substr(input, j, len);
	if (input[i - len] == '$')
	{
		string.tmp3 = dollar(0, string.tmp2, cmd);
		tmp = ft_strjoin(*new_input, string.tmp3);
		free(string.tmp3);
	}
	else
		tmp = ft_strjoin(*new_input, string.tmp2);
	if (*new_input)
		free(*new_input);
	*new_input = ft_strdup(tmp);
	free(tmp);
	free(string.tmp2);
	return (len - 1);
}

int	single_quotes(char **new_input, char *input, int i, t_cmd *cmd)
{
	int		j;
	int		len;
	char	*tmp;

	len = 0;
	j = i + 1;
	while (input[++i] && input[i] != '\'')
		len++;
	if (input[i] == '\0')
		print_error("double quotes error\n", 1, cmd);
	tmp = ft_substr(input, j, len);
	if (!ft_strncmp(*new_input, "", 1))
		free(*new_input);
	*new_input = ft_strjoin(*new_input, tmp);
	return (len + 1);
}

char	*double_quotes2(char *tmp, t_cmd *cmd)
{
	int			i;
	int			j;
	t_string	string;

	i = -1;
	while (tmp[++i])
	{
		if (tmp[i] == '$')
		{
			j = 1;
			while (tmp[++i] && tmp[i] != ' ' && tmp[i] != '$' && \
			tmp[i] != '\'' && tmp[i] != '?')
				j++;
			string.tmp1 = ft_substr(tmp, 0, i - j);
			string.tmp3 = ft_substr(tmp, i - j, j);
			string.mid = dollar(0, string.tmp3, cmd);
			string.tmp2 = ft_substr(tmp, i, ft_strlen(tmp));
			if (tmp)
				free(tmp);
			tmp = ft_strjoin(ft_strjoin(string.tmp1, string.mid), string.tmp2);
			i = ft_strlen(string.tmp1) + ft_strlen(string.mid);
		}
	}
	return (tmp);
}

int	double_quotes(char **new_input, char *input, int i, t_cmd *cmd)
{
	int		j;
	int		len;
	char	*tmp;
	char	*tmp2;

	len = 0;
	j = i + 1;
	while (input[++i] && input[i] != '"')
		len++;
	if (input[i] == '\0')
		print_error("double quotes error\n", 1, cmd);
	tmp = ft_substr(input, j, len);
	tmp2 = double_quotes2(tmp, cmd);
	if (*new_input)
		free(*new_input);
	*new_input = ft_strjoin(*new_input, tmp2);
	return (len + 1);
}

char	*handle_quotes(char *input, t_cmd *cmd)
{
	int				i;
	int				len;
	char			*new_input;

	i = -1;
	new_input = ft_strdup("");
	while (input[++i])
	{
		if (input[i] == '"')
			i += double_quotes(&new_input, input, i, cmd);
		if (input[i] == '\'')
			i += single_quotes(&new_input, input, i, cmd);
		if (input[i] && input[i] != '"' && input[i] != '\'')
			i += not_quotes(&new_input, input, i, cmd);
	}
	return (new_input);
}
