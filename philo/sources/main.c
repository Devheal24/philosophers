/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 15:56:53 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/16 23:48:49 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <bits/pthreadtypes.h>
#include <unistd.h>

unsigned long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

int	main(int argc, char **argv)
{
	t_data		*data;
	t_philo		*philo;
	pthread_t	*threads;
	int			i;

	if (error_number_of_arguments(argc) || args_not_valid(argv))
		return (1);
	data = initialize_structure(argv, argc);
	if (!data)
		return (1);
	philo = malloc(sizeof(t_philo) * data->nb_philo);
	if (!philo)
	{
		free_structure(data);
		return (1);
	}
	threads = malloc(sizeof(pthread_t) * data->nb_philo);
	if (!threads)
	{
		free(philo);
		free_structure(data);
		return (1);
	}
	i = 0;
	while (i < data->nb_philo)
	{
		philo[i].id = i;
		philo[i].data = data;
		pthread_create(&threads[i], NULL, routine, &philo[i]);
		i++;
	}
	i = 0;
	while (i < data->nb_philo)
		pthread_join(threads[i++], NULL);
	free(philo);
	free(threads);
	free_structure(data);
	return (0);
}
