/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:40:40 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/26 16:44:34 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_not_a_valid_number(char *argv)
{
	long	result;
	int		i;

	i = 0;
	result = 0;
	while (is_space(argv[i]))
		i++;
	if (argv[i] == '+')
		i++;
	if (argv[i] == '-')
		return (1);
	while (argv[i] >= '0' && argv[i] <= '9')
	{
		result = (result * 10) + (argv[i] - '0');
		if (result < 0)
			return (1);
		i++;
	}
	if (argv[i] != '\0' || result > INT_MAX)
		return (1);
	return (0);
}

int	args_not_valid(char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (argv[i][0] == '\0')
		{
			printf(RED "The argument number %d is empty\n" RESET, i);
			return (1);
		}
		else if (is_not_a_valid_number(argv[i]))
		{
			printf("'%s'" RED ": is not a valid number\n" RESET, argv[i]);
			return (1);
		}
		i++;
	}
	return (0);
}
