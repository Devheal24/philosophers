/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 15:56:53 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/12 17:26:42 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	error_number_of_arguments(int argc)
{
	if (argc < 5)
	{
		write(2, "There are not enough arguments\n", 31);
		return (1);
	}
	if (argc > 6)
	{
		write(2, "There are too much arguments\n", 29);
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	(void)argv;
	if (error_number_of_arguments(argc))
		return (1);
	return (0);
}
