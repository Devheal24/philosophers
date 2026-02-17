/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 15:27:54 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/17 13:15:58 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_structure(t_data *data)
{
	int	i;

	i = 0;
	while (data->fork && i < data->nb_philo)
	{
		pthread_mutex_destroy(&data->fork[i]);
		i++;
	}
	pthread_mutex_destroy(&data->mutex);
	pthread_mutex_destroy(&data->wait);
	if (data->threads)
		free(data->threads);
	if (data->fork)
		free(data->fork);
	free(data);
	data = NULL;
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
	pthread_mutex_init(&data->wait, NULL);
	pthread_mutex_init(&data->mutex, NULL);
	data->fork = NULL;
	data->threads = NULL;
	if (argc == 6)
		data->rotation = ft_atou(argv[5]);
	else
		data->rotation = -1;
	data->fork = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->fork)
	{
		free_structure(data);
		return (NULL);
	}
	i = 0;
	while (i < data->nb_philo)
		pthread_mutex_init(&data->fork[i++], NULL);
	data->threads = malloc(sizeof(pthread_t) * data->nb_philo);
	if (!data->threads)
	{
		free_structure(data);
		return (NULL);
	}
	return (data);
}
