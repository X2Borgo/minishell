/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alborghi <alborghi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 11:14:26 by alborghi          #+#    #+#             */
/*   Updated: 2025/03/21 11:14:34 by alborghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include "libft/libft.h"

int	printf(const char *n, ...);
int	ft_putbase(unsigned long n, char *base);
int	ft_putbase_long(unsigned long n, char *base);

#endif