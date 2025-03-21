/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_multiarg.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-sant <fde-sant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:00:58 by fde-sant          #+#    #+#             */
/*   Updated: 2025/03/21 18:33:54 by fde-sant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//trova i nuovi argomenti da aggiungere
char	**find_new_arg(t_words *words, int j, t_data *data)
{
	char	*first_part;
	char	*last_part;
	char	**arr;
	int		i;

	first_part = dup_till_n(words->word, j, data);
	last_part = ft_strdup(&words->word[j]);
	arr = quote_split(first_part, ' ');
	i = 0;
	while (arr[i] != NULL)
		i++;
	if (i > 0)
		arr[i - 1] = ft_strjoin_free_1(arr[i - 1], last_part);
	return (free(first_part), free(last_part), arr);
}

//gestisce il caso in cui l'espanzione della variabile deve creare più argomenti
t_words	*multi_args_case(t_data *data, t_words *words, int *j)
{
	t_words	*next;
	char	**arr;
	int		pos;
	int		i;

	next = words->next;
	pos = ft_strlen_int(words->word) - *j;
	arr = find_new_arg(words, *j, data);
	i = 0;
	if (arr[0] != NULL)
	{
		free (words->word);
		words->word = arr[0];
	}
	words->pipe = 0;
	while (arr[0] != NULL && arr[++i] != NULL)
		words = new_word(words, arr[i], data);
	if (arr[0] != NULL && arr[1] != NULL)
		words->next = next;
	if (i > 0)
		*j = ft_strlen_int(arr[i - 1]) - pos;
	return (free (arr), words);
}
