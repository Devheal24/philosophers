/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_fork.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 19:32:30 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/18 19:32:52 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	lock_fork(t_data *data, t_philo *philo, int first)
{
	int	left;
	int	right;

	left = philo->id;
	right = (philo->id + 1) % data->nb_philo;
	if (philo->id == 0 || philo->id + 1 == data->nb_philo)
		usleep(100);
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
