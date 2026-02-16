/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 15:27:54 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/16 22:21:51 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>

void	free_structure(t_philo *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_destroy(&data->fork[i]);
		i++;
	}
	pthread_mutex_destroy(&data->mutex);
	free(data->philo);
	free(data->fork);
	free(data);
}

t_philo	*initialize_structure(char **argv, int argc)
{
	t_philo	*data;

	data = malloc(sizeof(t_philo));
	if (!data)
		return (NULL);
	data->died = 0;
	data->id_philo = 1;
	data->nb_philo = ft_atou(argv[1]);
	data->time_to_die = ft_atou(argv[2]);
	data->time_to_eat = ft_atou(argv[3]);
	data->time_to_sleep = ft_atou(argv[4]);
	data->start_time = get_time_in_ms();
	if (argc == 6)
		data->rotation = ft_atou(argv[5]);
	else
		data->rotation = -1;
	data->philo = (pthread_t *)malloc(sizeof(pthread_t) * data->nb_philo);
	if (data->philo)
		data->fork = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (data->fork)
		return (data);
	if (data->philo)
		free(data->philo);
	free(data);
	return (NULL);
}
