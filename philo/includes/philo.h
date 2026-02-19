/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 15:57:41 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/19 15:46:09 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <limits.h>

# define RED "\033[1;31m"
# define PURPLE "\033[35m"
# define BLUE "\033[34m"
# define GREEN "\033[1;32m"
# define BROWN "\033[33m"
# define RESET "\033[0m"

# define THOUSAND 1000
# define THINK 0
# define FORK 1
# define EAT 2
# define SLEEP 3
# define DIE 4

typedef struct s_data
{
	int				nb_philo;
	unsigned long	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
	unsigned long	start_time;
	int				rotation;
	int				died;
	pthread_t		*threads;
	pthread_mutex_t	mutex;
	pthread_mutex_t	*fork;
}	t_data;

typedef struct s_philo
{
	t_data			*data;
	int				id;
	int				number_of_eating;
	unsigned long	start_rotation;
}	t_philo;

// PARSING
int				args_not_valid(char **argv);
int				error_number_of_arguments(int argc);

// STRUCTURE
t_data			*initialize_structure(char **argv, int argc);
t_data			*free_structure(t_data *data);

// UTIlS
int				is_space(char c);
int				ft_strlen(char *s);
unsigned long	ft_atou(char *tab);

// GET TIME
unsigned long	get_time_in_ms(void);

// ROUTINE
void			*routine(void *arg);
void			message(t_data *data, int id, int step);
int				a_philo_is_die(t_data *data, t_philo *philo, int code);

// ACTIONS
void			lock_fork(t_data *data, t_philo *philo, int first);
void			unlock_fork(t_data *data, t_philo *philo, int first);
int				is_thinking(t_data *data, t_philo *philo);
int				is_sleeping(t_data *data, t_philo *philo);
int				is_eating(t_data *data, t_philo *philo);

#endif