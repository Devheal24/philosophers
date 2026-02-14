/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 15:57:41 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/14 23:30:15 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>

# define RED "\033[1;31m"
# define BLUE "\033[1;34m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define RESET "\033[0m"

typedef struct s_philo
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	long			start_time;
	int				rotation;
	pthread_t		*philo;
	pthread_mutex_t	*fork;
}	t_philo;

// PARSING
int				args_not_valid(char **argv);

// ERROR MANAGEMENT
int				error_number_of_arguments(int argc);

// UTIlS
int				is_space(char c);
int				ft_strlen(char *s);
unsigned int	ft_atou(char *tab);

#endif