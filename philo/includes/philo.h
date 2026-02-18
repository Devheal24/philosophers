/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 15:57:41 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/18 13:01:16 by mgarnier         ###   ########.fr       */
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
	unsigned long	start_rotation;
}	t_philo;

// PARSING
int				args_not_valid(char **argv);
t_data			*initialize_structure(char **argv, int argc);
void			free_structure(t_data *data);

// ERROR MANAGEMENT
int				error_number_of_arguments(int argc);

// UTIlS
int				is_space(char c);
int				ft_strlen(char *s);
unsigned long	ft_atou(char *tab);

// GET TIME
unsigned long	get_time_in_ms(void);

void			*routine(void *arg);

#endif