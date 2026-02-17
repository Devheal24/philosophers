/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 15:56:53 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/17 19:13:42 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
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
		pthread_create(&data->threads[i], NULL, routine, &philo[i]);
		i++;
	}
	data->start_time = get_time_in_ms();
	pthread_mutex_unlock(&data->mutex);
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
	start_thread(data, philo);
	free(philo);
	free_structure(data);
	return (0);
}
