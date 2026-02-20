/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 15:29:44 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/20 19:10:11 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	order_passage(t_data *data, t_philo *philo, int *first)
{
	if (data->time_to_die < data->time_to_eat || data->nb_philo == 1)
		return ;
	if (data->nb_philo % 2 == 0)
	{
		if (*first == 1 && (philo->id + 1) % 2 == 0)
			usleep(data->time_to_eat * THOUSAND);
		*first = 0;
	}
	if (data->nb_philo % 2 == 1)
	{
		if ((philo->id + 1) % 2 == 0)
			usleep(data->time_to_eat * THOUSAND);
		if (philo->id + 1 == data->nb_philo)
			usleep((data->time_to_eat + 1) * THOUSAND);
		if (*first == 0 && (philo->id + 1) % 2 == 1
			&& philo->id + 1 != data->nb_philo)
			usleep(data->time_to_eat * THOUSAND);
		*first = 0;
	}
}

void	*routine(void *arg)
{
	t_philo	*philo;
	int		first;

	philo = (t_philo *)arg;
	first = 1;
	pthread_mutex_lock(&philo->data->mutex);
	philo->number_of_eating = philo->data->rotation;
	philo->start_rotation = get_time_in_ms();
	pthread_mutex_unlock(&philo->data->mutex);
	while (1)
	{
		if (is_thinking(philo->data, philo))
			break ;
		order_passage(philo->data, philo, &first);
		if (is_eating(philo->data, philo))
			break ;
		pthread_mutex_lock(&philo->data->mutex);
		philo->number_of_eating--;
		pthread_mutex_unlock(&philo->data->mutex);
		if (philo->number_of_eating == 0)
			break ;
		if (is_sleeping(philo->data, philo))
			break ;
	}
	return (NULL);
}
