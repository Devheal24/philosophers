/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 11:07:15 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/14 18:13:53 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

int	error_number_of_arguments(int argc)
{
	if (argc < 5)
		printf("There are " RED "not enough" RESET " arguments\n");
	if (argc > 6)
		printf("There are " RED "too much" RESET " arguments\n");
	if (argc < 5 || argc > 6)
		return (1);
	return (0);
}
