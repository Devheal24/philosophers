/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 15:27:54 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/25 14:00:08 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	free_structure(t_data *data)
{
	sem_close(data->fork);
	sem_unlink("/my_fork");
	sem_close(data->sem);
	sem_unlink("/my_sem");
	sem_close(data->watchdog);
	sem_unlink("/my_dog");
	if (data->pid)
		free(data->pid);
	free(data);
}

static t_data	*open_sem_t(t_data *data)
{
	data->sem = sem_open("/my_sem", O_CREAT | O_EXCL, 0644, 1);
	if (data->sem == SEM_FAILED)
	{
		free(data);
		return (NULL);
	}
	data->fork = sem_open("/my_fork", O_CREAT | O_EXCL, 0644, data->nb_philo);
	if (data->fork == SEM_FAILED)
	{
		sem_close(data->sem);
		sem_unlink("/my_sem");
		free(data);
		return (NULL);
	}
	data->watchdog = sem_open("/my_dog", O_CREAT | O_EXCL, 0644, 1);
	if (data->watchdog == SEM_FAILED)
	{
		sem_close(data->sem);
		sem_unlink("/my_sem");
		sem_close(data->fork);
		sem_unlink("/my_fork");
		free(data);
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
	sem_unlink("/my_sem");
	sem_unlink("/my_fork");
	sem_unlink("/my_dog");
	data = open_sem_t(data);
	if (!data)
		return (NULL);
	data->pid = malloc(sizeof(pid_t) * data->nb_philo);
	if (!data->pid)
	{
		free_structure(data);
		return (NULL);
	}
	return (data);
}
