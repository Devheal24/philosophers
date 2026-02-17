/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 15:56:53 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/17 13:20:54 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

void	*start_thread(void *arg)
{
	t_data	*data;
	t_philo	*philo;
	int		i;

	data = (t_data *)arg;
	philo = malloc(sizeof(t_philo) * data->nb_philo);
	if (!philo)
	{
		write(2, "error allocation failed\n", 24);
		return (NULL);
	}
	pthread_mutex_lock(&data->wait);
	i = 0;
	while (i < data->nb_philo)
	{
		philo[i].id = i;
		philo[i].data = data;
		pthread_create(&data->threads[i], NULL, routine, &philo[i]);
		i++;
	}
	data->start_time = get_time_in_ms();
	pthread_mutex_unlock(&data->wait);
	i = 0;
	while (i < data->nb_philo)
		pthread_join(data->threads[i++], NULL);
	free(philo);
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_data		*data;
	pthread_t	thread;

	if (error_number_of_arguments(argc) || args_not_valid(argv))
		return (1);
	data = initialize_structure(argv, argc);
	if (!data)
		return (1);
	pthread_create(&thread, NULL, start_thread, data);
	pthread_join(thread, NULL);
	free_structure(data);
	return (0);
}
