#ifndef TIME_H
#define TIME_H

#include <stdbool.h>
#include <sys/types.h>
#include <pthread.h>
#include "t_time_info.h"
#include "t_philo.h"

t_time_info get_time_info(void);
t_time_info set_time_info_once(t_time_info info);
void usleep_ms(unsigned int time_in_ms,bool death,pthread_mutex_t *death_mtx);
long gettimeofday_in_ms();

#endif /* TIME_H */
