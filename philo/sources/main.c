/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 15:56:53 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/16 22:00:28 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

unsigned long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

int	main(int argc, char **argv)
{
	t_philo	*data;
	int		i;

	if (error_number_of_arguments(argc) || args_not_valid(argv))
		return (1);
	data = initialize_structure(argv, argc);
	if (!data)
		return (1);
	i = 0;
	while (i < data->nb_philo)
		pthread_mutex_init(&data->fork[i++], NULL);
	pthread_mutex_init(&data->mutex, NULL);
	i = 0;
	while (i < data->nb_philo)
	{
		pthread_create(&data->philo[i++], NULL, routine, data);
		usleep(150);
	}
	i = 0;
	while (i < data->nb_philo)
		pthread_join(data->philo[i++], NULL);
	free_structure(data);
	return (0);
}
