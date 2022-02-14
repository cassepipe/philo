#ifndef T_PHILO_H
#define T_PHILO_H

#include <pthread.h>
#include <stdbool.h>
#include "t_fork.h"

typedef struct s_philo {
	pthread_t		thread;
	int				id;
	bool			*start;
	bool			dead;
	pthread_mutex_t *death_mtx;
	t_fork			 *left_fork;
	t_fork			 *right_fork;
}	t_philo;

#endif /* T_PHILO_H */
