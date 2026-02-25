/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 15:29:44 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/25 20:29:34 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	free_all_and_exit(t_philo *philo, int i)
{
	if (philo[i].nb_eating == 0)
	{
		free_structure(philo->data);
		free(philo);
		exit(0);
	}
	free_structure(philo->data);
	free(philo);
	exit(1);
}

static void	wait_calculator(t_data *data, t_philo *philo, int *first)
{
	if (data->time_to_eat >= data->time_to_sleep)
	{
		if ((philo->id + 1) % 2 == 0)
			usleep((data->time_to_eat) * THOUSAND);
		else if (philo->id + 1 == data->nb_philo)
			usleep((data->time_to_eat + 1) * THOUSAND);
		else if (*first == 0 && (philo->id + 1) % 2 == 1)
		{
			if (get_time_in_ms() + data->time_to_eat
				>= philo->start_rotation + data->time_to_die)
			{
				usleep((philo->start_rotation + data->time_to_die + 1
						- get_time_in_ms()) * THOUSAND);
			}
			else
				usleep(data->time_to_eat * THOUSAND);
		}
	}
	else
	{
		if (*first == 1 && (philo->id + 1) % 2 == 0)
			usleep((data->time_to_eat) * THOUSAND);
		else if (*first == 1 && philo->id + 1 == data->nb_philo)
			usleep((data->time_to_eat + 1) * THOUSAND);
	}
}

static void	order_passage(t_data *data, t_philo *philo, int *first)
{
	if (data->nb_philo == 1)
		return ;
	if (data->time_to_eat > data->time_to_die)
		if ((philo->id + 1) % 2 == 0)
			usleep(data->time_to_die * THOUSAND);
	if (data->nb_philo % 2 == 0)
	{
		if (*first == 1 && (philo->id + 1) % 2 == 0)
			usleep(data->time_to_eat * THOUSAND);
		else if (*first == 0 && (philo->id + 1) % 2 == 1
			&& data->time_to_die / 2 < data->time_to_eat)
			usleep((data->start_time + data->time_to_die
					- get_time_in_ms()) * THOUSAND);
	}
	if (data->nb_philo % 2 == 1)
	{
		wait_calculator(data, philo, first);
	}
	*first = 0;
}

static void	wait_until_all_finish_meal(t_data *data, t_philo *philo, int i)
{
	sem_wait(data->sem);
	if (data->nb_philo % 2 == 0 && data->died == 0)
	{
		sem_post(data->sem);
		if ((philo[i].id + 1) % 2 == 1)
			usleep(data->time_to_eat * THOUSAND);
	}
	else if (data->nb_philo % 2 == 1 && data->died == 0)
	{
		sem_post(data->sem);
		if ((philo[i].id + 1) % 2 == 1 && philo[i].id + 1 != data->nb_philo)
			usleep((data->time_to_eat * 2) * THOUSAND);
		else if ((philo[i].id + 1) % 2 == 0)
			usleep((data->time_to_eat) * THOUSAND);
	}
	else
		sem_post(data->sem);
	sem_post(data->watchdog);
}

void	*routine(t_philo *philo, int i, int first)
{
	pthread_t	thread;
	pthread_t	dog;

	philo[i].nb_eating = philo[i].data->rotation;
	philo[i].start_rotation = get_time_in_ms();
	pthread_create(&thread, NULL, monitoring_child, &philo[i]);
	pthread_create(&dog, NULL, watchdog, &philo[i]);
	while (1)
	{
		if (is_thinking(philo[i].data, &philo[i]))
			break ;
		order_passage(philo[i].data, &philo[i], &first);
		if (is_eating(philo[i].data, &philo[i]))
			break ;
		sem_wait(philo[i].data->sem);
		philo[i].nb_eating--;
		sem_post(philo[i].data->sem);
		if (philo[i].nb_eating == 0 || is_sleeping(philo[i].data, &philo[i]))
			break ;
	}
	pthread_join(thread, NULL);
	wait_until_all_finish_meal(philo->data, philo, i);
	pthread_join(dog, NULL);
	free_all_and_exit(philo, i);
	exit(0);
}
