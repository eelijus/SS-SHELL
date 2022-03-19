/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sujilee <sujilee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 14:21:45 by sean              #+#    #+#             */
/*   Updated: 2022/03/19 16:20:37 by sujilee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_glob	g_glob;

int	not_quotes(char **new_input, char *input, int i, t_cmd *cmd)
{
	int		j;
	int		len;
	char	*tmp;
	char	*tmp2;
	char	*tmp3;
	t_string	str;

	len = 1;
	j = i;
	while (input[++i] && input[i] != ' ' && input[i] != '"' && \
	input[i] != '\'' && input[i] != '$')
		len++;
	if (input[i - len] == '$')
	{
		//sujilee free
		str.four = ft_substr(input, j, len);
		tmp = ft_strjoin(*new_input, dollar(0, str.four, cmd));
		if (input[i - len + 1] == '?')
			len = 2;
		//sujilee free
		free(str.four);
	}
	else
	{
		//sujilee free
		str.five = ft_substr(input, j, len);
		tmp = ft_strjoin(*new_input, str.five);
		//sujilee free
		free(str.five);
	}
	if (!ft_strncmp(*new_input, "", ft_strlen(*new_input)))
		free(*new_input);
	*new_input = ft_strdup(tmp);
	if (!tmp2)
		free(tmp2);
	if (!tmp)
		free(tmp);
	if (!tmp3)
		free(tmp3);
	return (len - 1);
}

int	single_quotes(char **new_input, char *input, int i, t_cmd *cmd)
{
	int		j;
	int		len;
	char	*tmp;

	g_glob.quote = 1;
	len = 0;
	j = i + 1;
	while (input[++i] && input[i] != '\'')
		len++;
	if (input[i] == '\0')
		print_error("single quotes error\n", 1, cmd);
	if (g_glob.exit_status == 1)
		return (0);
	tmp = ft_substr(input, j, len);
	if (!ft_strncmp(*new_input, "", 1))
		free(*new_input);
	*new_input = ft_strjoin(*new_input, tmp);
	//sujilee free
	free(tmp);
	return (len + 1);
}

char	*handle_quotes3(char **new_input)
{
	g_glob.exit_status = 0;
	if (ft_strncmp(*new_input, "", ft_strlen(*new_input)))
		free(*new_input);
	return (ft_strdup(""));
}

char	*double_quotes2(char *tmp, t_cmd *cmd)
{
	int		i;
	int		j;
	char	*mid;
	char	*t1;
	char	*t2;

	i = -1;
	while (tmp[++i])
	{
		if (tmp[i] == '$')
		{
			j = 1;
			while (tmp[++i] && tmp[i] != ' ' && tmp[i] != '$' && \
			tmp[i] != '\'' && tmp[i] != '?')
				j++;
			t1 = ft_substr(tmp, 0, i - j);
			mid = dollar(0, ft_substr(tmp, i - j, j), cmd);
			t2 = ft_substr(tmp, i, ft_strlen(tmp));
			if (tmp)
				free(tmp);
			tmp = ft_strjoin(ft_strjoin(t1, mid), t2);
			i = ft_strlen(t1) + ft_strlen(mid) - 1;
			//sujilee free
			free(t2);
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

	g_glob.quote = 1;
	len = 0;
	j = i + 1;
	while (input[++i] && input[i] != '"')
		len++;
	if (input[i] == '\0')
		print_error("double quotes error\n", 1, cmd);
	if (g_glob.exit_status == 1)
		return (0);
	tmp = ft_substr(input, j, len);
	tmp2 = double_quotes2(tmp, cmd);
	if (!ft_strncmp(*new_input, "", 1))
		free(*new_input);
	*new_input = ft_strjoin(*new_input, tmp2);
	//sujilee free
	free(tmp);
	return (len + 1);
}

char	*handle_quotes2(char *input, t_cmd *cmd)
{
	int		i;
	char	*new_input;

	i = -1;
	new_input = ft_strdup("");
	while (input[++i])
	{
		if (input[i] == '"')
			i += double_quotes(&new_input, input, i, cmd);
		if (g_glob.exit_status == 1 && g_glob.quote)
			break ;
		if (input[i] == '\'')
			i += single_quotes(&new_input, input, i, cmd);
		if (g_glob.exit_status == 1 && g_glob.quote)
			break ;
		if (input[i] && input[i] != '"' && input[i] != '\'')
			i += not_quotes(&new_input, input, i, cmd);
	}
	if (ft_strncmp(new_input, "exit", ft_strlen("exit")) && \
	g_glob.exit_status == 1 && g_glob.quote == 1)
		return (handle_quotes3(&new_input));
	else
		return (new_input);
}

char	*handle_quotes(char *input, t_cmd *cmd, int process)
{
	int				i;
	int				len;
	char			*new_input;
	char			**input_split;

	if (!input)
		return (input);
	i = -1;
	input_split = ft_split(input, ' ');
	if (!ft_strncmp(cmd->cmd, "echo", ft_strlen("echo")))
		process = 0;
	// if ((!ft_strncmp(input_split[0], "export", ft_strlen(input_split[0])) && \
	// !ft_strncmp(input_split[0], "export", ft_strlen("export"))) || \
	// (!ft_strncmp(input_split[0], "cat", ft_strlen(input_split[0])) && \
	// !ft_strncmp(input_split[0], "cat", ft_strlen("cat"))))
	if (process == 1 && (!ft_strchr(cmd->cmd, '"' || !ft_strchr(cmd->cmd, '\''))))
	{
		free(input_split);
		return (ft_strdup(input));
	}
	new_input = handle_quotes2(input, cmd);
	return (new_input);
}
