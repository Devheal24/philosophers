/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 15:56:53 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/14 23:36:23 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

unsigned long	get_time_in_ms(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

void	*routine(void *arg)
{
	t_philo 		*data;
	unsigned long	timestamp;
	unsigned long	is_die;

	data = (t_philo *)arg;
	is_die = get_time_in_ms() + (data->time_to_die * 1000);
	timestamp = get_time_in_ms();
	while (timestamp < is_die)
	{
		timestamp = get_time_in_ms();
		printf(RED"%lu philo [] has taken a fork\n"RESET, timestamp % 100000);
		printf(GREEN"%lu philo [] is eating\n"RESET, timestamp % 100000);
		is_die = get_time_in_ms() + data->time_to_die;
		usleep(data->time_to_eat * 1000);
		timestamp = get_time_in_ms();
		printf(BLUE"%lu philo [] is sleeping\n"RESET, timestamp % 100000);
		usleep(data->time_to_sleep * 1000);
		timestamp = get_time_in_ms();
		printf(YELLOW"%lu philo [] is thinking\n"RESET, timestamp % 100000);
		timestamp = get_time_in_ms();
	}
	printf(RED"%lu philo [] died\n"RESET, timestamp % 100000);
	return (NULL);
}

t_philo	*init_struct(char **argv, int argc)
{
	t_philo *data;

	data = malloc(sizeof(t_philo));
	if (!data)
		return (NULL);
	data->nb_philo = ft_atou(argv[1]);
	data->time_to_die = ft_atou(argv[2]);
	data->time_to_eat = ft_atou(argv[3]);
	data->time_to_sleep = ft_atou(argv[4]);
	data->start_time = get_time_in_ms();
	if (argc == 6)
		data->rotation = ft_atou(argv[5]);
	else
	 	data->rotation = -1;
	data->philo = (pthread_t *)malloc(sizeof(pthread_t) * data->nb_philo);
	if (data->philo)
		data->fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (data->fork)
		return (data);
	if (data->philo)
		free(data->philo);
	free(data);
	return (NULL);
}

void	free_structure(t_philo *data)
{
	free(data->philo);
	int i = 0;
	while (i < data->nb_philo)
	{
		pthread_mutex_destroy(&data->fork[i]);
		i++;
	}
	free(data->fork);
	free(data);
}

int	main(int argc, char **argv)
{
	t_philo 	*data;

	if (error_number_of_arguments(argc))
		return (1);
	if (args_not_valid(argv))
		return (1);
	data = init_struct(argv, argc);
	if (!data)
		return (1);
	int i = 0;
	while (i < data->nb_philo)
		pthread_mutex_init(&data->fork[i++], NULL);
	i = 0;
	while (i < data->nb_philo)
		pthread_create(&data->philo[i++], NULL, routine, data);
	i = 0;
	while (i < data->nb_philo)
		pthread_join(data->philo[i++], NULL);
	free_structure(data);
	return (0);
}
