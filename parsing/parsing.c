/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alborghi <alborghi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 09:57:09 by fre007            #+#    #+#             */
/*   Updated: 2025/03/21 11:16:48 by alborghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//se stai veramente leggendo questa descrizione ho paura per il tuo QI
int	count_args(t_words **words)
{
	t_words	*tmp;
	int		i;

	if ((*words)->pipe == 0)
		(*words) = (*words)->next;
	tmp = *words;
	i = 0;
	while (tmp != NULL && tmp->pipe == 0)
	{
		tmp = tmp->next;
		i++;
	}
	return (i);
}

//alloca un nodo di cmds tutto impostato a NULL
t_cmd	*calloc_cmds(t_data *data)
{
	t_cmd	*cmds;

	cmds = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmds)
		ft_exit(data, 1);
	cmds->file_o = NULL;
	cmds->file_a = NULL;
	cmds->file_i = NULL;
	cmds->delimiter = NULL;
	cmds->divider = NULL;
	cmds->args = NULL;
	cmds->cmd = NULL;
	cmds->here_file = NULL;
	cmds->next = NULL;
	return (cmds);
}

//scrive il comando all'interno di un nodo della lista cmds
void	command_slicer(t_cmd *cmds, t_words **words, t_data *data, t_words **h)
{
	int		i;
	int		j;

	(*words) = inout_manager(*words, data, cmds, h);
	if (data->status || (*words) == NULL)
		return ;
	set_cmd(cmds, words);
	i = count_args(words);
	cmds->args = (char **)malloc(sizeof(char *) * (i + 1));
	if (!(cmds->args))
		ft_exit(data, 1);
	cmds->args[i] = NULL;
	j = -1;
	while (--i >= 0)
	{
		cmds->args[++j] = (*words)->word;
		(*words) = (*words)->next;
	}
	cmds->divider = NULL;
	if ((*words) != NULL && !(*words)->took)
		cmds->divider = (*words)->word;
	if ((*words) != NULL)
		(*words)->took = 1;
}

//crea un nuovo nodo per la lista cmds
t_cmd	*new_command(t_cmd *cmds, t_words **words, t_data *data, t_words **h)
{
	t_cmd	*new_cmd;

	new_cmd = calloc_cmds(data);
	cmds->next = new_cmd;
	command_slicer(new_cmd, words, data, h);
	return (new_cmd);
}

//crea la lista contenente la lista cmds facendo il parsing necessario
t_cmd	*parsing(char *line, t_data *data)
{
	t_words	*words;
	t_words	*head;
	t_cmd	*cmds;
	t_cmd	*first;

	if (line[0] == '\0' || line == NULL || line[0] == '\n')
		return (NULL);
	if (check_syntax_error(line, data))
		return (NULL);
	cmds = calloc_cmds(data);
	words = word_slicer(line, data);
	head = words;
	command_slicer(cmds, &words, data, &head);
	first = cmds;
	while (!data->status && words != NULL)
	{
		if (words->pipe == 1 && words->took == 1)
			words = words->next;
		else
			cmds = new_command(cmds, &words, data, &head);
	}
	return (free_words_only_pointers(head), first);
}
