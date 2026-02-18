/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 15:56:53 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/18 16:37:41 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

void	monitoring(t_data *data, t_philo *philo)
{
	unsigned long	timing;
	int				i;

	while (data->nb_philo > 0)
	{
		i = 0;
		pthread_mutex_lock(&data->mutex);
		while (i < data->nb_philo)
		{
			if (philo[data->nb_philo - 1].number_of_eating == 0)
			{
				pthread_mutex_unlock(&data->mutex);
				return ;
			}
			timing = get_time_in_ms();
			if (timing > philo[i].start_rotation + data->time_to_die)
			{
				timing -= data->start_time;
				data->died = 1;
				message(data, philo, DIE);
				pthread_mutex_unlock(&data->mutex);
				return ;
			}
			i++;
		}
		pthread_mutex_unlock(&data->mutex);
	}
}

void	*start_thread(t_data *data, t_philo *philo)
{
	int	i;

	pthread_mutex_lock(&data->mutex);
	i = 0;
	while (i < data->nb_philo)
	{
		philo[i].id = i;
		philo[i].data = data;
		philo[i].start_rotation = data->start_time + data->time_to_die;
		pthread_create(&data->threads[i], NULL, routine, &philo[i]);
		i++;
	}
	data->start_time = get_time_in_ms();
	pthread_mutex_unlock(&data->mutex);
	monitoring(data, philo);
	i = 0;
	while (i < data->nb_philo)
		pthread_join(data->threads[i++], NULL);
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_data	*data;
	t_philo	*philo;

	if (error_number_of_arguments(argc) || args_not_valid(argv))
		return (1);
	data = initialize_structure(argv, argc);
	if (!data)
	{
		write(2, "error initialization\n", 21);
		return (1);
	}
	philo = malloc(sizeof(t_philo) * data->nb_philo);
	if (!philo)
	{
		write(2, "error allocation memory failed\n", 31);
		free_structure(data);
		return (1);
	}
	if (data->rotation != 0)
		start_thread(data, philo);
	free(philo);
	free_structure(data);
	return (0);
}
