/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 15:29:44 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/17 13:22:31 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_eating(t_data *data, int id)
{
	unsigned long	timestamp;
	unsigned long	time_start;
	int				left;
	int				right;

	left = id;
	right = (id + 1) % data->nb_philo;
	timestamp = get_time_in_ms();
	time_start = timestamp - data->start_time;
	if (id % 2 == 1)
		pthread_mutex_lock(&data->fork[left]);
	else
		pthread_mutex_lock(&data->fork[right]);
	timestamp = get_time_in_ms();
	time_start = timestamp - data->start_time;
	printf(PURPLE"[%lu ms] philo [%d] has taken a fork\n"RESET, time_start, id + 1);
	if (data->nb_philo == 1)
	{
		usleep(data->time_to_die * THOUSAND);
		pthread_mutex_unlock(&data->fork[left]);
		return (1);
	}
	else if (id % 2 == 1)
		pthread_mutex_lock(&data->fork[right]);
	else
		pthread_mutex_lock(&data->fork[left]);
	timestamp = get_time_in_ms();
	time_start = timestamp - data->start_time;
	printf(PURPLE"[%lu ms] philo [%d] has taken a fork\n"RESET, time_start, id + 1);
	printf(GREEN"[%lu ms] philo [%d] is eating\n"RESET, time_start, id + 1);
	if (data->time_to_eat > data->time_to_die)
	{
		usleep(data->time_to_die * THOUSAND);
		pthread_mutex_unlock(&data->fork[left]);
		pthread_mutex_unlock(&data->fork[right]);
		return (1);
	}
	else
		usleep(data->time_to_eat * THOUSAND);
	if (id % 2 == 1)
		pthread_mutex_unlock(&data->fork[left]);
	else
		pthread_mutex_unlock(&data->fork[right]);
	if (id % 2 == 1)
		pthread_mutex_unlock(&data->fork[right]);
	else
		pthread_mutex_unlock(&data->fork[left]);
	return (0);
}

int	is_sleeping(t_data *data, int id)
{
	unsigned long	timestamp;
	unsigned long	time_start;

	timestamp = get_time_in_ms();
	time_start = timestamp - data->start_time;
	printf(BLUE"[%lu ms] philo [%d] is sleeping\n"RESET, time_start, id + 1);
	if (data->time_to_sleep > data->time_to_die - data->time_to_eat)
	{
		usleep((data->time_to_die - data->time_to_eat) * THOUSAND);
		return (1);
	}
	else
		usleep(data->time_to_sleep * THOUSAND);
	usleep(100);
	return (0);
}

int	is_thinking(t_data *data, int id)
{
	unsigned long	timestamp;
	unsigned long	time_start;

	timestamp = get_time_in_ms();
	time_start = timestamp - data->start_time;
	printf(YELLOW"[%lu ms] philo [%d] is thinking\n"RESET, time_start, id + 1);
	return (0);
}

void	is_die(t_data *data, int id)
{
	unsigned long	timestamp;
	unsigned long	time_start;

	timestamp = get_time_in_ms();
	time_start = timestamp - data->start_time;
	pthread_mutex_lock(&data->mutex);
	if (data->died == 0)
		printf(RED"[%lu ms] philo [%d] died\n"RESET, time_start, id + 1);
	data->died = 1;
	pthread_mutex_unlock(&data->mutex);
}

void	*routine(void *arg)
{
	t_philo			*philo;
	t_data			*data;
	int				rotation;

	philo = (t_philo *)arg;
	data = (t_data *)philo->data;
	pthread_mutex_lock(&data->wait);
	pthread_mutex_unlock(&data->wait);
	rotation = data->rotation;
	while (rotation != 0)
	{
		if (is_thinking(data, philo->id))
			break ;
		if (is_eating(data, philo->id))
			break ;
		rotation--;
		if (rotation == 0)
			break ;
		if (is_sleeping(data, philo->id))
			break ;
	}
	if (rotation != 0)
		is_die(data, philo->id);
	return (NULL);
}
