/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 15:29:44 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/18 19:40:18 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	a_philo_is_die(t_data *data, t_philo *philo, int code)
{
	if (data->died != 0)
	{
		unlock_fork(data, philo, code);
		pthread_mutex_unlock(&data->mutex);
		return (1);
	}
	return (0);
}

static int	is_eating(t_data *data, t_philo *philo)
{
	lock_fork(data, philo, 1);
	pthread_mutex_lock(&data->mutex);
	if (a_philo_is_die(data, philo, 1))
		return (1);
	message(data, philo->id, FORK);
	pthread_mutex_unlock(&data->mutex);
	if (data->nb_philo == 1)
	{
		unlock_fork(data, philo, 1);
		return (1);
	}
	lock_fork(data, philo, 2);
	pthread_mutex_lock(&data->mutex);
	if (a_philo_is_die(data, philo, 2))
		return (1);
	message(data, philo->id, FORK);
	message(data, philo->id, EAT);
	philo->start_rotation = get_time_in_ms();
	pthread_mutex_unlock(&data->mutex);
	usleep(data->time_to_eat * THOUSAND);
	unlock_fork(data, philo, 2);
	return (0);
}

static int	is_sleeping(t_data *data, t_philo *philo)
{
	pthread_mutex_lock(&data->mutex);
	if (data->died != 0)
	{
		pthread_mutex_unlock(&data->mutex);
		return (1);
	}
	message(data, philo->id, SLEEP);
	pthread_mutex_unlock(&data->mutex);
	usleep(data->time_to_sleep * THOUSAND);
	return (0);
}

static int	is_thinking(t_data *data, t_philo *philo)
{
	pthread_mutex_lock(&data->mutex);
	if (data->died != 0)
	{
		pthread_mutex_unlock(&data->mutex);
		return (1);
	}
	message(data, philo->id, THINK);
	pthread_mutex_unlock(&data->mutex);
	return (0);
}

void	*routine(void *arg)
{
	t_philo			*philo;
	t_data			*data;

	philo = (t_philo *)arg;
	data = (t_data *)philo->data;
	pthread_mutex_lock(&data->mutex);
	philo->number_of_eating = data->rotation;
	philo->start_rotation = get_time_in_ms();
	pthread_mutex_unlock(&data->mutex);
	while (1)
	{
		if (is_thinking(data, philo))
			break ;
		if (is_eating(data, philo))
			break ;
		pthread_mutex_lock(&data->mutex);
		philo->number_of_eating--;
		pthread_mutex_unlock(&data->mutex);
		if (philo->number_of_eating == 0)
			break ;
		if (is_sleeping(data, philo))
			break ;
	}
	return (NULL);
}
