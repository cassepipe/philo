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
void usleep_ms(unsigned int time_in_ms, t_philo *philo);
t_time_info get_time_info(void);
long gettimeofday_in_ms();
void eat(t_philo *philo);
void print_philo(const t_philo *philo,const char *action,int act_sz);
void print_dead_philo(const t_philo *philo,const char *action,int act_sz);
void	print_philo_death(const t_philo *philo);

#endif /* ROUTINES_H */
