/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inout_support.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-sant <fde-sant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 09:52:47 by fre007            #+#    #+#             */
/*   Updated: 2025/03/21 19:22:32 by fde-sant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//trova un stringa in un'altra stringa
char	*ft_strstr(char *big, char *little)
{
	int	i;
	int	j;
	int	check;

	i = 0;
	while (little != NULL && big[i])
	{
		check = quote_checker(big, i);
		j = 0;
		while (big[i + j] == little[j] && little[j] && !check)
			j++;
		if (!little[j])
		{
			quote_checker("1", 1);
			return (&big[i]);
		}
		i++;
	}
	quote_checker("1", 1);
	return (NULL);
}

//rimuove un nodo della lista ricollegandola
t_words	*remove_node_words(t_words *words, t_words *first)
{
	t_words	*tmp;

	if (first == words)
	{
		tmp = words->next;
		free (words->word);
		free (words);
		return (tmp);
	}
	while (first && first->next != words)
		first = first->next;
	if (first == NULL)
		return (first);
	tmp = words;
	first->next = words->next;
	free (words->word);
	words = words->next;
	free (tmp);
	return (words);
}

//rimuove il pezzo di stringa contenente il finded
char	*remove_finded(char *word, char *finded, t_data *data)
{
	int		i;
	char	*new_word;

	i = -1;
	new_word = ft_calloc(1, ft_strlen(word) - ft_strlen(finded) + 1);
	if (!new_word)
		ft_exit(data, 1);
	while (word[++i])
	{
		if (ft_strlen(&word[i]) == ft_strlen(finded))
			break ;
		new_word[i] = word[i];
	}
	return (free (word), new_word);
}

//protegge la head mantenendo all'elemento corretto
int	head_protector(t_words *words, t_words **h, char *find)
{
	if (ft_strstr((*h)->word, find))
		return (1);
	if (ft_strstr(words->word, find))
	{
		while ((*h) != words && (*h)->next != NULL && (*h)->next != words)
			(*h) = (*h)->next;
		return (2);
	}
	return (0);
}

//controlla che noncia siano problemi con i file di output
int	check_out_error(t_cmd *cmds)
{
	if (cmds->file_a == NULL && cmds->file_o == NULL)
		return (1);
	if (!ft_strncmp(cmds->file_a, "\"\"", 3)
		|| !ft_strncmp(cmds->file_a, "\'\'", 3))
		cmds->out_error = 1;
	if (!ft_strncmp(cmds->file_o, "\"\"", 3)
		|| !ft_strncmp(cmds->file_o, "\'\'", 3))
		cmds->out_error = 1;
	return (1);
}
