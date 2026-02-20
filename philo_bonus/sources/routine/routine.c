/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 15:29:44 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/20 19:12:38 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
	sem_wait(philo->data->sem);
	philo->number_of_eating = philo->data->rotation;
	philo->start_rotation = get_time_in_ms();
	sem_post(philo->data->sem);
	while (1)
	{
		if (is_thinking(philo->data, philo))
			break ;
		order_passage(philo->data, philo, &first);
		if (is_eating(philo->data, philo))
			break ;
		sem_wait(philo->data->sem);
		philo->number_of_eating--;
		sem_post(philo->data->sem);
		if (philo->number_of_eating == 0)
			break ;
		if (is_sleeping(philo->data, philo))
			break ;
	}
	return (NULL);
}
