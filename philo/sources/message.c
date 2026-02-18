/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 14:18:50 by mgarnier          #+#    #+#             */
/*   Updated: 2026/02/18 15:05:21 by mgarnier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	message(t_data *data, t_philo *philo, int step)
{
	unsigned long	timing;
	int				id;

	timing = get_time_in_ms() - data->start_time;
	id = philo->id + 1;
	if (step == 0)
		printf(BROWN"[%lu ms] philo [%d] is thinking\n"RESET, timing, id);
	if (step == 1)
		printf(PURPLE"[%lu ms] philo [%d] has taken a fork\n"RESET, timing, id);
	else if (step == 2)
		printf(GREEN"[%lu ms] philo [%d] is eating\n"RESET, timing, id);
	else if (step == 3)
		printf(BLUE"[%lu ms] philo [%d] is sleeping\n"RESET, timing, id);
	else if (step == 4)
		printf(RED"[%lu ms] philo [%d] died\n"RESET, timing, id);
}
