/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 15:29:44 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/17 18:29:25 by mgarnier         ###   ########.fr       */
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

int	a_thread_is_dead(t_data *data, t_philo *philo, int code)
{
	pthread_mutex_lock(&data->mutex);
	if (data->died != 0)
	{
		unlock_fork(data, philo, code);
		pthread_mutex_unlock(&data->mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->mutex);
	return (0);
}

int	is_eating(t_data *data, t_philo *philo)
{
	unsigned long	timestamp;
	unsigned long	time_start;

	timestamp = get_time_in_ms();
	if (a_thread_is_dead(data, philo, 0))
		return (1);
	lock_fork(data, philo, 1);
	timestamp = get_time_in_ms();
	if (timestamp > philo->start_rotation + data->time_to_die)
	{
		if (philo->id % 2 == 1)
			pthread_mutex_unlock(&data->fork[philo->id]);
		else
			pthread_mutex_unlock(&data->fork[(philo->id + 1) % data->nb_philo]);
		return (1);
	}
	time_start = timestamp - data->start_time;
	if (a_thread_is_dead(data, philo, 1))
		return (1);
	printf(PURPLE"[%lu ms] philo [%d] has taken a fork\n"RESET, time_start, philo->id + 1);
	if (data->nb_philo == 1)
	{
		usleep(data->time_to_die * THOUSAND);
		pthread_mutex_unlock(&data->fork[philo->id]);
		return (1);
	}
	lock_fork(data, philo, 0);
	timestamp = get_time_in_ms();
	if (timestamp > philo->start_rotation + data->time_to_die)
	{
		unlock_fork(data, philo, 2);
		return (1);
	}
	time_start = timestamp - data->start_time;
	if (a_thread_is_dead(data, philo, 2))
		return (1);
	printf(PURPLE"[%lu ms] philo [%d] has taken a fork\n"RESET, time_start, philo->id + 1);
	printf(GREEN"[%lu ms] philo [%d] is eating\n"RESET, time_start, philo->id + 1);
	philo->start_rotation = get_time_in_ms();
	if (data->time_to_eat > data->time_to_die)
	{
		usleep(data->time_to_die * THOUSAND);
		unlock_fork(data, philo, 2);
		return (1);
	}
	usleep(data->time_to_eat * THOUSAND);
	unlock_fork(data, philo, 2);
	return (0);
}

int	is_sleeping(t_data *data, t_philo *philo)
{
	unsigned long	timestamp;
	unsigned long	time_start;

	timestamp = get_time_in_ms();
	if (timestamp > philo->start_rotation + data->time_to_die)
		return (1);
	time_start = timestamp - data->start_time;
	if (a_thread_is_dead(data, philo, 0))
		return (1);
	printf(BLUE"[%lu ms] philo [%d] is sleeping\n"RESET, time_start, philo->id + 1);
	if (data->time_to_sleep > data->time_to_die - data->time_to_eat)
	{
		usleep((data->time_to_die - data->time_to_eat) * THOUSAND);
		return (1);
	}
	else
		usleep(data->time_to_sleep * THOUSAND);
	return (0);
}

int	is_thinking(t_data *data, t_philo *philo)
{
	unsigned long	timestamp;
	unsigned long	time_start;

	timestamp = get_time_in_ms();
	if (timestamp > philo->start_rotation + data->time_to_die)
		return (1);
	time_start = timestamp - data->start_time;
	if (a_thread_is_dead(data, philo, 0))
		return (1);
	printf(BROWN"[%lu ms] philo [%d] is thinking\n"RESET, time_start, philo->id + 1);
	return (0);
}

void	is_die(t_data *data, t_philo *philo)
{
	unsigned long	timestamp;
	unsigned long	time_start;

	timestamp = get_time_in_ms();
	time_start = philo->start_rotation + data->time_to_die - data->start_time;
	pthread_mutex_lock(&data->mutex);
	if (data->died == 0)
	{
		printf(RED"[%lu ms] philo [%d] died\n"RESET, time_start, philo->id + 1);
		data->died = 1;
	}
	pthread_mutex_unlock(&data->mutex);
}

void	*routine(void *arg)
{
	t_philo			*philo;
	t_data			*data;
	int				rotation;

	philo = (t_philo *)arg;
	data = (t_data *)philo->data;
	rotation = data->rotation;
	pthread_mutex_lock(&data->mutex);
	pthread_mutex_unlock(&data->mutex);
	philo->start_rotation = get_time_in_ms();
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
	if (rotation != 0)
		is_die(data, philo);
	return (NULL);
}
