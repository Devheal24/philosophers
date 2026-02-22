/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 15:27:54 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/22 22:38:35 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	free_structure(t_data *data)
{
	sem_close(data->fork);
	sem_unlink("/my_fork");
	sem_close(data->sem);
	sem_unlink("/my_sem");
	if (data->threads)
		free(data->threads);
	free(data);
}

t_data	*open_sem_t(t_data *data)
{
	sem_unlink("/my_sem");
	data->sem = sem_open("/my_sem", O_CREAT | O_EXCL, 0644, 1);
	if (data->sem == SEM_FAILED)
	{
		free(data);
		return (NULL);
	}
	sem_unlink("/my_fork");
	data->fork = sem_open("/my_fork", O_CREAT | O_EXCL, 0644, data->nb_philo);
	if (data->fork == SEM_FAILED)
	{
		free(data);
		sem_close(data->sem);
		sem_unlink("/my_sem");
		return (NULL);
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
	data = open_sem_t(data);
	data->threads = malloc(sizeof(pthread_t) * data->nb_philo);
	if (!data->threads)
	{
		free_structure(data);
		return (NULL);
	}
	return (data);
}
