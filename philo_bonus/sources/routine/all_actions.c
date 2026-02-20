/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_actions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 15:42:49 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/20 18:48:05 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	a_philo_is_die(t_data *data, t_philo *philo, int code)
{
	if (data->died != 0)
	{
		unlock_fork(data, philo, code);
		sem_post(data->sem);
		return (1);
	}
	return (0);
}

int	is_eating(t_data *data, t_philo *philo)
{
	lock_fork(data, philo, 1);
	sem_wait(data->sem);
	if (a_philo_is_die(data, philo, 1))
		return (1);
	message(data, philo->id, FORK);
	sem_post(data->sem);
	if (data->nb_philo == 1)
	{
		unlock_fork(data, philo, 1);
		return (1);
	}
	lock_fork(data, philo, 2);
	sem_wait(data->sem);
	if (a_philo_is_die(data, philo, 2))
		return (1);
	message(data, philo->id, FORK);
	message(data, philo->id, EAT);
	philo->start_rotation = get_time_in_ms();
	sem_post(data->sem);
	if (data->time_to_die > data->time_to_eat)
		usleep(data->time_to_eat * THOUSAND);
	else
		usleep((data->time_to_die + 1) * THOUSAND);
	unlock_fork(data, philo, 2);
	return (0);
}

int	is_sleeping(t_data *data, t_philo *philo)
{
	sem_wait(data->sem);
	if (data->died != 0)
	{
		sem_post(data->sem);
		return (1);
	}
	message(data, philo->id, SLEEP);
	sem_post(data->sem);
	if (data->time_to_die > data->time_to_sleep + data->time_to_eat)
		usleep(data->time_to_sleep * THOUSAND);
	else
		usleep((data->time_to_die - data->time_to_eat + 1) * THOUSAND);
	return (0);
}

int	is_thinking(t_data *data, t_philo *philo)
{
	sem_wait(data->sem);
	if (data->died != 0)
	{
		sem_post(data->sem);
		return (1);
	}
	message(data, philo->id, THINK);
	sem_post(data->sem);
	return (0);
}
