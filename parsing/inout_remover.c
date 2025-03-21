/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inout_remover.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-sant <fde-sant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 09:53:37 by fre007            #+#    #+#             */
/*   Updated: 2025/03/21 19:24:18 by fde-sant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//rimuove due nodi, l'attuale ed il sucecssivo
char	*clear_2_node(t_words **tmp, t_words **first, t_data *data)
{
	char	*ret;
	t_words	*tmp_first;

	tmp_first = *first;
	if ((*tmp) == (*first))
		(*first) = (*first)->next;
	(*tmp) = remove_node_words(*tmp, tmp_first);
	ret = ft_strdup((*tmp)->word);
	if (ret == NULL)
		ft_exit(data, 1);
	tmp_first = *first;
	if ((*tmp) == (*first))
		(*first) = (*first)->next;
	(*tmp) = remove_node_words(*tmp, tmp_first);
	if (!ft_strncmp("<<", data->find, 3))
		return (ret);
	return (dollar_manager_stupid(ret, data));
}

//rimuove il nodo subito successivoinsieme ad una parte della stringa
char	*clear_next_node(t_words **tmp, char *finded, t_words **first,
							t_data *data)
{
	char	*ret;
	t_words	*tmp_first;

	tmp_first = *first;
	ret = ft_strdup((*tmp)->next->word);
	if (ret == NULL)
		ft_exit(data, 1);
	remove_node_words((*tmp)->next, tmp_first);
	(*tmp)->word = remove_finded((*tmp)->word, finded, data);
	if (!ft_strncmp("<<", data->find, 3))
		return (ret);
	return (dollar_manager_stupid(ret, data));
}

//rimuove un nodo e riccolega la lista
char	*clear_this_node(t_words **tmp, char *finded, t_words **first,
							t_data *data)
{
	char	*ret;
	t_words	*tmp_first;

	tmp_first = *first;
	ret = ft_strdup(finded);
	if (ret == NULL)
		ft_exit(data, 1);
	if ((*tmp) == (*first))
		(*first) = (*tmp)->next;
	(*tmp) = remove_node_words(*tmp, tmp_first);
	if (!ft_strncmp("<<", data->find, 3))
		return (ret);
	return (dollar_manager_stupid(ret, data));
}

//rimuove l'ultima parte di una stringa
char	*remove_last_part(t_words **tmp, char *finded, char *find, t_data *data)
{
	char	*ret;

	ret = ft_strdup(&finded[ft_strlen(find)]);
	if (ret == NULL)
		ft_exit(data, 1);
	(*tmp)->word = remove_finded((*tmp)->word, finded, data);
	if (!ft_strncmp("<<", data->find, 3))
		return (ret);
	return (dollar_manager_stupid(ret, data));
}

//supporto per la funzione open useless file
void	open_file_sup(char *s, t_cmd *cmds)
{
	int	fd;

	if (!cmds->out_error)
	{
		fd = open(s, O_CREAT, 0644);
		if (fd == -1)
			return ;
		close(fd);
	}
	free (s);
	s = NULL;
}
