/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inout.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-sant <fde-sant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 20:21:59 by fre007            #+#    #+#             */
/*   Updated: 2025/03/21 19:21:08 by fde-sant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//prende quello subito dopo alla stringa find
char	*find_after_word(char *find, t_words **tmp, t_data *data)
{
	t_words	*first;
	char	*arr[2];

	first = (*tmp);
	while ((*tmp) != NULL && ((*tmp)->pipe == 0 || (*tmp) == first))
	{
		arr[1] = ft_strstr((*tmp)->word, find);
		if (arr[1] != NULL)
			break ;
		(*tmp) = (*tmp)->next;
	}
	if (arr[1] == NULL || (!arr[1][ft_strlen(find)] && (*tmp)->next == NULL))
		return ((*tmp) = first, NULL);
	arr[0] = NULL;
	if (ft_strlen(find) == ft_strlen((*tmp)->word))
		arr[0] = clear_2_node(tmp, &first, data);
	else if (ft_strlen(find) == ft_strlen(arr[1]))
		arr[0] = clear_next_node(tmp, arr[1], &first, data);
	else if (ft_strlen((*tmp)->word) == ft_strlen(arr[1]))
		arr[0] = clear_this_node(tmp, &arr[1][ft_strlen(find)], &first, data);
	else if (arr[1][ft_strlen(find)])
		arr[0] = remove_last_part(tmp, arr[1], find, data);
	return ((*tmp) = first, arr[0]);
}

char	*findable_file(t_words *words, int witch)
{
	t_words	*tmp;
	int		i;

	tmp = words;
	while (tmp != NULL && (tmp->pipe == 0 || (tmp == words && witch)))
	{
		i = 0;
		while (tmp->word != NULL && tmp->word[i]
			&& ((quote_checker(tmp->word, i)
					&& ft_strchr("<>", tmp->word[i]))
				|| !ft_strchr("<>", tmp->word[i])))
			i++;
		if (tmp->word == NULL || !tmp->word[i])
		{
			tmp = tmp->next;
			continue ;
		}
		if (tmp->word[i] != tmp->word[i + 1])
			return (ft_strndup(&(tmp->word[i]), 1));
		else
			return (ft_strndup(&(tmp->word[i]), 2));
		tmp = tmp->next;
	}
	quote_checker("1", 1);
	return (NULL);
}

// apre e chiude un file che non viene utilizzato
void	open_useless_file(t_cmd *cmds, int witch)
{
	if (witch == 1)
		cmds->file_a = NULL;
	else if (witch == 2)
		cmds->file_o = NULL;
	if (cmds->file_o != NULL)
		open_file_sup(cmds->file_o, cmds);
	else if (cmds->file_a != NULL)
		open_file_sup(cmds->file_a, cmds);
}

//gestisce la sovrascittura di nuovi nomi file
void	check_file(char *find, t_words **words, t_cmd *cmds, t_data *data)
{
	char	*finded;

	finded = find_after_word(find, words, data);
	if (finded == NULL)
		return ;
	if (ft_strncmp(find, ">>", 3) == 0)
	{
		open_useless_file(cmds, 2);
		cmds->file_a = finded;
	}
	else if (ft_strncmp(find, ">", 2) == 0)
	{
		open_useless_file(cmds, 1);
		cmds->file_o = finded;
	}
	if (ft_strncmp(find, "<<", 3) == 0)
	{
		cmds->doi = 2;
		cmds->delimiter = ft_append_line(cmds->delimiter, finded);
	}
	else if (ft_strncmp(find, "<", 2) == 0)
	{
		cmds->doi = 1;
		cmds->file_i = ft_append_line(cmds->file_i, finded);
	}
}

//verifica tutte le informazioni per i simboli: <, <<, >>, >
t_words	*inout_manager(t_words *words, t_data *data, t_cmd *cmds, t_words **h)
{
	char	*find;
	t_words	*tmp;
	t_words	*tmp_h;
	int		witch;

	cmds->doi = 0;
	find = findable_file(words, 1);
	tmp_h = *h;
	witch = head_protector(words, &tmp_h, find);
	while (check_out_error(cmds) && !data->status && find != NULL)
	{
		data->find = find;
		check_file(find, &words, cmds, data);
		free (find);
		find = findable_file(words, 0);
	}
	tmp = words;
	while (tmp != NULL && !tmp->pipe)
		tmp = dollar_manager(data, tmp);
	if (witch == 1)
		(*h) = words;
	else if (witch == 2)
		tmp_h->next = words;
	return (words);
}
