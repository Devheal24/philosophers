/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 15:31:20 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/16 19:26:57 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	is_space(char c)
{
	if ((c >= 9 && c <= 13) || c == ' ')
		return (1);
	return (0);
}

unsigned int	ft_atou(char *tab)
{
	int				i;
	unsigned int	result;

	i = 0;
	result = 0;
	if (!tab)
		return (0);
	while (tab[i] == 32 || (tab[i] > 8 && tab[i] < 14))
		i++;
	if (tab[i] == '+')
		i++;
	while (tab[i] == '0')
		i++;
	while (tab[i] >= '0' && tab[i] <= '9')
	{
		result = result * 10 + (tab[i] - 48);
		i++;
	}
	return (result);
}
