/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 15:56:53 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/25 00:55:03 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	monitoring(t_data *data, t_philo *philo)
{
	pid_t	dead;
	int		status;
	int		i;

	dead = waitpid(-1, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
	{
		i = 0;
		while (i < data->nb_philo)
		{
			sem_post(data->watchdog);
			if (dead == data->pid[i])
				message(data, philo[i].id, DIE);
			i++;
		}
		sem_post(data->sem);
	}
	i = 0;
	while (i < data->nb_philo)
		waitpid(data->pid[i++], NULL, 0);
}

static void	*start_forks(t_data *data, t_philo *philo)
{
	int	i;

	i = 0;
	sem_wait(data->watchdog);
	data->start_time = get_time_in_ms();
	while (i < data->nb_philo)
	{
		philo[i].id = i;
		philo[i].data = data;
		philo[i].start_rotation = data->start_time + data->time_to_die;
		philo[i].number_of_eating = -1;
		data->pid[i] = fork();
		if (data->pid[i] < 0)
		{
			write (2, "fork failed\n", 12);
			sem_post(data->watchdog);
			return (NULL);
		}
		if (data->pid[i] == 0)
			routine(philo, i, 1);
		i++;
	}
	if (data->nb_philo != 0)
		monitoring(data, philo);
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
		start_forks(data, philo);
	free_structure(data);
	free(philo);
	return (0);
}
