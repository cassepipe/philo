#ifndef T_PHILO_H
#define T_PHILO_H

#include <pthread.h>
#include <stdbool.h>

typedef struct s_philo {
	pthread_t		thread;
	int				id;
	bool			*start;
	bool			dead;
	pthread_mutex_t *death_mtx;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
}	t_philo;

#endif /* T_PHILO_H */
