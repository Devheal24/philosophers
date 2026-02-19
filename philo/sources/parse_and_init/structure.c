/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 15:27:54 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/18 19:30:15 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_data	*free_structure(t_data *data)
{
	int	i;

	i = 0;
	while (data->fork && i < data->nb_philo)
	{
		pthread_mutex_destroy(&data->fork[i]);
		i++;
	}
	pthread_mutex_destroy(&data->mutex);
	if (data->threads)
		free(data->threads);
	if (data->fork)
		free(data->fork);
	free(data);
	return (NULL);
}

t_data	*initialize_structure(char **argv, int argc)
{
	t_data	*data;
	int		i;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->died = 0;
	data->nb_philo = ft_atou(argv[1]);
	data->time_to_die = ft_atou(argv[2]);
	data->time_to_eat = ft_atou(argv[3]);
	data->time_to_sleep = ft_atou(argv[4]);
	data->start_time = get_time_in_ms();
	pthread_mutex_init(&data->mutex, NULL);
	data->rotation = (argc == 6) * ft_atou(argv[5]) + (argc == 5) * -1;
	data->fork = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	data->threads = malloc(sizeof(pthread_t) * data->nb_philo);
	if (!data->fork || !data->threads)
		data = free_structure(data);
	i = 0;
	if (data)
		while (i < data->nb_philo)
			pthread_mutex_init(&data->fork[i++], NULL);
	return (data);
}
