#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>
#include <sys/types.h>
#include <pthread.h>
#include "t_time_info.h"
#include "t_philo.h"

void *routine(void *arg);
t_time_info set_time_info_once(t_time_info info);
void init_all_philo(t_philo *philosophers,int nb_philo,pthread_mutex_t *mutexes,bool *start_uneven,bool *start_even);
void check_death(bool dead,pthread_mutex_t *death_mtx);
t_philo init_philo(int id,bool *start,pthread_mutex_t *death_mtx,pthread_mutex_t *left_fork,pthread_mutex_t *right_fork);

#endif /* MAIN_H */
