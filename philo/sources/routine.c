/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 15:29:44 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/16 21:59:00 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <unistd.h>

int	is_eating(t_philo *data, int id)
{
	unsigned long	timestamp;
	unsigned long	time_start;

	timestamp = get_time_in_ms();
	time_start = timestamp - data->start_time;
	printf(TRY"[%lu ms] philo [%d] has taken a fork\n"RESET, time_start, id);
	printf(GREEN"[%lu ms] philo [%d] is eating\n"RESET, time_start, id);
	if (data->time_to_eat > data->time_to_die)
	{
		usleep(data->time_to_die * THOUSAND);
		return (1);
	}
	else
		usleep(data->time_to_eat * THOUSAND);
	return (0);
}

int	is_sleeping(t_philo *data, int id)
{
	unsigned long	timestamp;
	unsigned long	time_start;

	timestamp = get_time_in_ms();
	time_start = timestamp - data->start_time;
	printf(BLUE"[%lu ms] philo [%d] is sleeping\n"RESET, time_start, id);
	if (data->time_to_sleep > data->time_to_die - data->time_to_eat)
	{
		usleep((data->time_to_die - data->time_to_eat) * THOUSAND);
		return (1);
	}
	else
		usleep(data->time_to_sleep * THOUSAND);
	return (0);
}

void	*routine(void *arg)
{
	t_philo			*data;
	unsigned long	timestamp;
	unsigned long	time_start;
	int				rotation;
	int				id;

	data = (t_philo *)arg;
	rotation = data->rotation;
	pthread_mutex_lock(&data->mutex);
	id = data->id_philo++;
	pthread_mutex_unlock(&data->mutex);
	timestamp = get_time_in_ms();
	while (rotation != 0)
	{
		if (is_eating(data, id))
			break ;
		if (is_sleeping(data, id))
			break ;
		timestamp = get_time_in_ms();
		time_start = timestamp - data->start_time;
		printf(YELLOW"[%lu ms] philo [%d] is thinking\n"RESET, time_start, id);
		rotation--;
	}
	timestamp = get_time_in_ms();
	if (rotation != 0)
		printf(RED"[%lu ms] philo [%d] died\n"RESET, time_start, id);
	return (NULL);
}
