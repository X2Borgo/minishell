/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fde-sant <fde-sant@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:20:43 by fde-sant          #+#    #+#             */
/*   Updated: 2025/03/21 17:58:42 by fde-sant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	**ft_count_words(char const *s, char c)
{
	size_t	words;
	size_t	i;
	char	**split;

	words = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c || quote_checker2((char *)s, i))
		{
			words++;
			while (s[i] != c && s[i] != '\0')
				i++;
		}
		else
			i++;
	}
	split = (char **)malloc((words + 1) * sizeof(char *));
	if (!split)
		return (quote_checker2("1", 1), NULL);
	split[0] = NULL;
	return (quote_checker2("1", 1), split);
}

static void	free_split(char **split)
{
	size_t	i;

	i = 0;
	while (split[i] != NULL)
	{
		free(split[i]);
		i++;
	}
	free(split);
}

static int	split_helper(char **split, const char *s, size_t dist, char c)
{
	size_t	j;

	j = 0;
	while (split[j] != NULL)
		j++;
	split[j] = ft_substr(s, 0, dist);
	if (!split[j])
	{
		free_split(split);
		return (-1);
	}
	j++;
	split[j] = NULL;
	while (s[dist] == c && s[dist] != '\0')
		dist++;
	return (dist);
}

char	*ft_strchr_quote(const char *s, int c)
{
	int				i;
	unsigned char	ch;

	i = 0;
	ch = (unsigned char)c;
	while ((s[i] != ch || quote_checker2((char *)s, i)) && s[i] != '\0')
		i++;
	if (s[i] == ch)
		return (quote_checker2("1", 1), (char *)&s[i]);
	return (quote_checker2("1", 1), NULL);
}

char	**quote_split(char const *s, char c)
{
	char	**split;
	char	*tmp;
	size_t	i;
	int		d;

	if (!s)
		return (NULL);
	split = ft_count_words(s, c);
	if (!split)
		return (NULL);
	i = 0;
	while (s[i] == c && s[i] != '\0')
		i++;
	while (s[i] != '\0')
	{
		tmp = ft_strchr_quote(&s[i], c);
		if (tmp)
			d = split_helper(split, &s[i], tmp - &s[i], c);
		else
			d = split_helper(split, &s[i], ft_strlen(&s[i]), c);
		if (d == -1)
			return (NULL);
		i += d;
	}
	return (split);
}
