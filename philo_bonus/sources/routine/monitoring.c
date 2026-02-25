/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 10:41:23 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/25 13:59:21 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*monitoring_child(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	while (data->nb_philo > 0)
	{
		sem_wait(data->sem);
		if (data->died == 1 || philo->number_of_eating == 0)
		{
			sem_post(data->sem);
			return (NULL);
		}
		if (get_time_in_ms() - philo->start_rotation >= data->time_to_die)
		{
			data->died = 1;
			sem_post(data->sem);
			return (NULL);
		}
		sem_post(data->sem);
		usleep(THOUSAND);
	}
	return (NULL);
}

void	*watchdog(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	sem_wait(philo->data->watchdog);
	sem_post(philo->data->watchdog);
	sem_wait(philo->data->sem);
	philo->data->died = 1;
	sem_post(philo->data->sem);
	return (NULL);
}
