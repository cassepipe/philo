#ifndef ROUTINES_H
#define ROUTINES_H

#include <stdbool.h>
#include <pthread.h>
#include "t_time_info.h"
#include "t_philo.h"
void check_death(bool dead,pthread_mutex_t *death_mtx);
void *routine(void *arg);
void think(t_philo *philo);
void sleeph(t_philo *philo);
void usleep_ms(unsigned int time_in_ms,bool death,pthread_mutex_t *death_mtx);
t_time_info get_time_info(void);
long gettimeofday_in_ms();
void print_philo(const t_philo *philo,const char *action,int act_sz);
void eat(t_philo *philo);

#endif /* ROUTINES_H */
