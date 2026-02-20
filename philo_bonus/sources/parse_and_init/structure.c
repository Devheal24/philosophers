/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 15:27:54 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/20 19:07:22 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <fcntl.h>
#include <semaphore.h>

void	free_structure(t_data *data)
{
	int	i;

	i = 0;
	while (data->fork && i < data->nb_philo)
		pthread_mutex_destroy(&data->fork[i++]);
	sem_close(data->sem);
	sem_unlink("/my_sem");
	if (data->threads)
		free(data->threads);
	if (data->fork)
		free(data->fork);
	free(data);
}

t_data	*alloc_and_init_forks(t_data *data)
{
	int	i;

	data->fork = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->fork)
	{
		free_structure(data);
		return (NULL);
	}
	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->fork[i], NULL) > 0)
		{
			while (i > 0)
				pthread_mutex_destroy(&data->fork[--i]);
			free(data->fork);
			free_structure(data);
			return (NULL);
		}
		i++;
	}
	return (data);
}

t_data	*initialize_structure(char **argv, int argc)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->died = 0;
	data->nb_philo = ft_atou(argv[1]);
	data->time_to_die = ft_atou(argv[2]);
	data->time_to_eat = ft_atou(argv[3]);
	data->time_to_sleep = ft_atou(argv[4]);
	data->start_time = get_time_in_ms();
	data->rotation = (argc == 6) * ft_atou(argv[5]) + (argc == 5) * -1;
	sem_unlink("/my_sem");
	data->sem = sem_open("/my_sem", O_CREAT | O_EXCL, 0644, 1);
	if (data->sem == SEM_FAILED)
	{
		free(data);
		return (NULL);
	}
	data->threads = malloc(sizeof(pthread_t) * data->nb_philo);
	if (!data->threads)
	{
		free_structure(data);
		return (NULL);
	}
	data = alloc_and_init_forks(data);
	return (data);
}
