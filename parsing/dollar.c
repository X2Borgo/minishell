/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-sant <fde-sant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 09:06:16 by fre007            #+#    #+#             */
/*   Updated: 2025/03/21 17:04:56 by fde-sant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//sostituisce la variabile indicata con il valore dell'env corrispondente
char	*copy_in_str(char *word, int *i, int j, t_data *data)
{
	char	*new_word;
	char	*env;
	char	*str;
	int		l;
	int		y;

	str = dup_till_n(&word[j + 1], *i - j - 1, data);
	env = get_env(data->env, str);
	new_word = ft_calloc(1, ft_strlen(env) + ft_strlen(&word[*i]) + j + 1);
	if (!new_word)
		ft_exit(data, 1);
	l = -1;
	while (++l != j)
		new_word[l] = word[l];
	j = -1;
	while (env != NULL && env[++j])
		new_word[l++] = env[j];
	y = *i;
	*i = l;
	while (word[y])
		new_word[l++] = word[y++];
	return (free (word), free (str), new_word);
}

//sostituisce la variabile che gli viene indicata tramite l'indice
t_words	*dollar_converter(char *word, int *i, t_data *data, t_words *words)
{
	int	j;

	if (*i > 0 && word[*i - 1] == '\\')
		return (words->word = remove_char(word, *i - 1, data), words);
	j = *i;
	if (word[*i + 1] == '?')
		words->word = status_report (words->word, i, data);
	else if (ft_isalnum(word[*i + 1]) || word[*i + 1] == '_')
	{
		*i += 1;
		if (!ft_isdigit(word[*i]))
		{
			while (ft_isalnum(word[*i]) || word[*i] == '_')
				*i += 1;
		}
		else
			*i += 1;
		words->word = copy_in_str(word, i, j, data);
		if (quote_checker(NULL, 0) == 0)
			return (multi_args_case(data, words, i));
	}
	else
		*i += 1;
	return (words);
}

//rimuove i $ prima dei ' e "
char	*dollar_remover(char *word, int *i, int check, t_data *data)
{
	if (word[*i] == '$' && (word[*i + 1] == '\'' || word[*i + 1] == '\"')
		&& (*i == 0 || word[*i - 1] != '\\') && check == 0)
		word = remove_char(word, *i, data);
	return (word);
}

//gestione il simbolo $ e le quote
t_words	*dollar_manager(t_data *data, t_words *words)
{
	int	i;
	int	check;
	int	pre;

	i = 0;
	check = 0;
	while (!data->status && words->word != NULL && words->word[i])
	{
		words->word = dollar_remover(words->word, &i, check, data);
		pre = check;
		check = quote_checker(words->word, i);
		if (check != pre)
			words->word = remove_char(words->word, i, data);
		else if (check != 1 && words->word[i] == '$'
			&& (i == 0 || words->word[i - 1] != '\\'))
			words = dollar_converter(words->word, &i, data, words);
		else if (((check != 1 && words->word[i] == '$')
				|| words->word[i] == '\'' || words->word[i] == '\"')
			&& i != 0 && words->word[i - 1] == '\\')
			words->word = remove_char(words->word, i - 1, data);
		else
			i++;
	}
	quote_checker("1", 1);
	return (words->next);
}
