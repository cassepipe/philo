#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#include <sys/time.h>

#include "t_time_info.h"

#include "time.h"

long	gettimeofday_in_ms()
{
	struct timeval tv;
	long	time_in_ms;

	gettimeofday(&tv, NULL);
	time_in_ms = tv.tv_sec * 1000;
	time_in_ms += tv.tv_usec / 1000;
	return (time_in_ms);
}
#define USLEEP_SPLIT	160

void	usleep_ms(unsigned int time_in_ms, bool death, pthread_mutex_t *death_mtx)
{
	unsigned long	time;
	int				i;

	i = 0;
	time = time_in_ms * 1000 / USLEEP_SPLIT;
	assert(time < 1000000);
	while (i < USLEEP_SPLIT && death == false)
	{
		if (death == true)
			pthread_mutex_lock(death_mtx);
		usleep(time);
		i++;
	}
}

t_time_info	set_time_info_once(t_time_info info)
{
	static	bool		defined;
	static	t_time_info time_info;

	if (!defined)
	{
		defined = true;
		time_info.time_to_die = info.time_to_die;
		time_info.time_to_eat = info.time_to_eat;
		time_info.time_to_sleep = info.time_to_sleep;
	}
	return (time_info);
}


t_time_info	get_time_info(void)
{
	return (set_time_info_once((t_time_info){0, 0, 0}));
}
