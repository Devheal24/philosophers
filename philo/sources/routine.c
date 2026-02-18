/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 15:29:44 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/18 12:57:00 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	lock_fork(t_data *data, t_philo *philo, int first)
{
	int				left;
	int				right;

	left = philo->id;
	right = (philo->id + 1) % data->nb_philo;
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
	int				left;
	int				right;

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
	unsigned long	timestamp;

	lock_fork(data, philo, 1);
	pthread_mutex_lock(&data->mutex);
	if (data->died == 0)
	{
		timestamp = get_time_in_ms() - data->start_time;
		printf(PURPLE"[%lu ms] philo [%d] has taken a fork\n"RESET, timestamp, philo->id + 1);
	}
	else
	{
		unlock_fork(data, philo, 1);
		pthread_mutex_unlock(&data->mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->mutex);
	lock_fork(data, philo, 2);
	pthread_mutex_lock(&data->mutex);
	if (data->died == 0)
	{
		timestamp = get_time_in_ms() - data->start_time;
		printf(PURPLE"[%lu ms] philo [%d] has taken a fork\n"RESET, timestamp, philo->id + 1);
		printf(GREEN"[%lu ms] philo [%d] is eating\n"RESET, timestamp, philo->id + 1);
		philo->start_rotation = get_time_in_ms();
		pthread_mutex_unlock(&data->mutex);
		usleep(data->time_to_eat * THOUSAND);
		unlock_fork(data, philo, 2);
		usleep(100);
	}
	else
	{
		unlock_fork(data, philo, 2);
		pthread_mutex_unlock(&data->mutex);
		return (1);
	}
	return (0);
}

int	is_sleeping(t_data *data, t_philo *philo)
{
	unsigned long	timestamp;

	pthread_mutex_lock(&data->mutex);
	if (data->died == 0)
	{
		timestamp = get_time_in_ms() - data->start_time;
		printf(BLUE"[%lu ms] philo [%d] is sleeping\n"RESET, timestamp, philo->id + 1);
		pthread_mutex_unlock(&data->mutex);
		usleep(data->time_to_sleep * THOUSAND);
	}
	else
	{
		pthread_mutex_unlock(&data->mutex);
		return (1);
	}
	return (0);
}

int	is_thinking(t_data *data, t_philo *philo)
{
	unsigned long	timestamp;

	pthread_mutex_lock(&data->mutex);
	if (data->died == 0)
	{
		timestamp = get_time_in_ms() - data->start_time;
		printf(BROWN"[%lu ms] philo [%d] is thinking\n"RESET, timestamp, philo->id + 1);
		pthread_mutex_unlock(&data->mutex);
	}
	else
	{
		pthread_mutex_unlock(&data->mutex);
		return (1);
	}
	return (0);
}

void	*routine(void *arg)
{
	t_philo			*philo;
	t_data			*data;
	int				rotation;

	philo = (t_philo *)arg;
	data = (t_data *)philo->data;
	pthread_mutex_lock(&data->mutex);
	rotation = data->rotation;
	philo->start_rotation = get_time_in_ms();
	pthread_mutex_unlock(&data->mutex);
	while (rotation != 0)
	{
		if (is_thinking(data, philo))
			break ;
		if (is_eating(data, philo))
			break ;
		rotation--;
		if (rotation == 0)
			break ;
		if (is_sleeping(data, philo))
			break ;
	}
	return (NULL);
}
