/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_fork.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpouget <tpouget@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:47:05 by tpouget           #+#    #+#             */
/*   Updated: 2022/02/14 14:47:05 by tpouget          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_FORK_H
#define T_FORK_H

#include <pthread.h>
#include <stdbool.h>

typedef struct s_fork	{
	pthread_mutex_t		fork_mtx;
	bool				fork_status;
}	t_fork;

#endif /* T_FORK_H */
