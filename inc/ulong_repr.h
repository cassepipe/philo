#ifndef ULONG_REPR_H
#define ULONG_REPR_H

#include <stdbool.h>
#include <pthread.h>
#include "t_time_info.h"
#include "t_philo.h"

char *ulong_repr(long n,char *buffer,int *len);

#endif /* ULONG_REPR_H */
