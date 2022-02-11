#ifndef PRINT_PHILO_H
#define PRINT_PHILO_H

#include <stdbool.h>
#include <pthread.h>
#include "t_time_info.h"
#include "t_philo.h"

long gettimeofday_in_ms();
char *ulong_repr(long n,char *buffer,int *len);
void print_philo(const t_philo *philo,const char *action,int act_sz);

#endif /* PRINT_PHILO_H */
