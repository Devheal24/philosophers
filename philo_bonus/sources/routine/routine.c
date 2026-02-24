/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 15:29:44 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/24 12:41:42 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	free_all_and_exit(t_philo *philo, int i)
{
	if (philo[i].number_of_eating == 0)
	{
		free_structure(philo->data);
		free(philo);
		exit(0);
	}
	free_structure(philo->data);
	free(philo);
	exit(1);
}

static void	order_passage(t_data *data, t_philo *philo, int *first)
{
	if (data->time_to_die < data->time_to_eat || data->nb_philo == 1)
		return ;
	if (data->nb_philo % 2 == 0)
		if (*first == 1 && (philo->id + 1) % 2 == 0)
			usleep(data->time_to_eat * THOUSAND);
	if (data->nb_philo % 2 == 1)
	{
		if ((philo->id + 1) % 2 == 0)
			usleep(data->time_to_eat * THOUSAND);
		if (philo->id + 1 == data->nb_philo)
			usleep((data->time_to_eat + 1) * THOUSAND);
		if (*first == 0 && (philo->id + 1) % 2 == 1
			&& philo->id + 1 != data->nb_philo)
		{
			if (get_time_in_ms() + data->time_to_eat
				>= philo->start_rotation + data->time_to_die)
				usleep((philo->start_rotation + data->time_to_die + 1
						- get_time_in_ms()) * THOUSAND);
			else
				usleep(data->time_to_eat * THOUSAND);
		}
	}
	*first = 0;
}

static void	*monitoring_child(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	while (data->nb_philo > 0)
	{
		sem_wait(data->sem);
		if (data->died == 1)
		{
			sem_post(data->sem);
			return (NULL);
		}
		if (philo->number_of_eating == 0
			|| get_time_in_ms() - philo->start_rotation >= data->time_to_die)
		{
			data->died = 1;
			sem_post(data->watchdog);
			sem_post(data->sem);
			return (NULL);
		}
		sem_post(data->sem);
		usleep(THOUSAND);
	}
	return (NULL);
}

static void	*watchdog(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	sem_wait(philo->data->watchdog);
	sem_post(philo->data->watchdog);
	sem_wait(philo->data->sem);
	philo->data->died = 1;
	sem_post(philo->data->sem);
	return (NULL);
}

void	*routine(t_philo *philo, int i, int first)
{
	pthread_t	thread;
	pthread_t	dog;

	philo[i].number_of_eating = philo[i].data->rotation;
	philo[i].start_rotation = get_time_in_ms();
	pthread_create(&thread, NULL, monitoring_child, &philo[i]);
	pthread_create(&dog, NULL, watchdog, &philo[i]);
	while (!philo[i].data->died)
	{
		if (is_thinking(philo[i].data, &philo[i]))
			break ;
		order_passage(philo[i].data, &philo[i], &first);
		if (is_eating(philo[i].data, &philo[i]))
			break ;
		sem_wait(philo[i].data->sem);
		philo[i].number_of_eating--;
		sem_post(philo[i].data->sem);
		if (philo[i].number_of_eating == 0
			|| is_sleeping(philo[i].data, &philo[i]))
			break ;
	}
	pthread_join(thread, NULL);
	pthread_join(dog, NULL);
	free_all_and_exit(philo, i);
	exit(0);
}
