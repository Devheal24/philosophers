/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 15:29:44 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/18 19:07:42 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	lock_fork(t_data *data, t_philo *philo, int first)
{
	int	left;
	int	right;

	left = philo->id;
	right = (philo->id + 1) % data->nb_philo;
	usleep(philo->id * 10);
	if (first == 1)
	{
		if (philo->id % 2 == 1)
			pthread_mutex_lock(&data->fork[left]);
		else
			pthread_mutex_lock(&data->fork[right]);
	}
	else
	{
		if (philo->id % 2 == 1)
			pthread_mutex_lock(&data->fork[right]);
		else
			pthread_mutex_lock(&data->fork[left]);
	}
}

void	unlock_fork(t_data *data, t_philo *philo, int first)
{
	int	left;
	int	right;

	left = philo->id;
	right = (philo->id + 1) % data->nb_philo;
	if (first == 0)
		;
	else if (first == 1)
	{
		if (philo->id % 2 == 1)
			pthread_mutex_unlock(&data->fork[left]);
		else
			pthread_mutex_unlock(&data->fork[right]);
	}
	else
	{
		pthread_mutex_unlock(&data->fork[right]);
		pthread_mutex_unlock(&data->fork[left]);
	}
}

int	is_eating(t_data *data, t_philo *philo)
{
	lock_fork(data, philo, 1);
	pthread_mutex_lock(&data->mutex);
	if (data->died != 0)
	{
		unlock_fork(data, philo, 1);
		pthread_mutex_unlock(&data->mutex);
		return (1);
	}
	message(data, philo->id, FORK);
	pthread_mutex_unlock(&data->mutex);
	if (data->nb_philo == 1)
	{
		unlock_fork(data, philo, 1);
		return (1);
	}
	lock_fork(data, philo, 2);
	pthread_mutex_lock(&data->mutex);
	if (data->died != 0)
	{
		unlock_fork(data, philo, 2);
		pthread_mutex_unlock(&data->mutex);
		return (1);
	}
	message(data, philo->id, FORK);
	message(data, philo->id, EAT);
	philo->start_rotation = get_time_in_ms();
	pthread_mutex_unlock(&data->mutex);
	usleep(data->time_to_eat * THOUSAND);
	unlock_fork(data, philo, 2);
	return (0);
}

int	is_sleeping(t_data *data, t_philo *philo)
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

int	is_thinking(t_data *data, t_philo *philo)
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
