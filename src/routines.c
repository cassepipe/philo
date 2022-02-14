#include <threads.h>
#include <unistd.h>

#include "t_philo.h"

#include "defines.h"
#include "routines.h"

void	eat(t_philo *philo)
{
	static thread_local long	last_time_I_ate;
	long 						time;
	long						diff;

	pthread_mutex_lock(philo->left_fork);
	print_philo(philo, FORK1_STR, sizeof(FORK1_STR));
	pthread_mutex_lock(philo->right_fork);
	print_philo(philo, FORK2_STR, sizeof(FORK2_STR));
	time = gettimeofday_in_ms();
	print_philo(philo, EAT_STR, sizeof(EAT_STR));
	if (last_time_I_ate != 0)
		diff = time - last_time_I_ate;
	else
		diff = 0;
	if (diff < get_time_info().time_to_die)
	{
		last_time_I_ate = time;
		usleep_ms(get_time_info().time_to_eat, philo);
	}
	else if  (philo->dead == false)
	{
		philo->dead = true;
		print_dead_philo(philo, DIE_STR, sizeof(DIE_STR));
	}
	pthread_mutex_unlock(philo->left_fork);
	/*printf("%li\tPhilo %i dropped left fork : %hx\n", gettimeofday_in_ms(), philo->id,  philo->left_fork);*/
	pthread_mutex_unlock(philo->right_fork);
	/*printf("%li\tPhilo %i dropped right fork : %hx\n", gettimeofday_in_ms(), philo->id, philo->right_fork);*/
}

void	sleeph(t_philo *philo)
{
	print_philo(philo, SLEEP_STR, sizeof(SLEEP_STR));
	usleep_ms(get_time_info().time_to_sleep, philo);
}

void	think(t_philo *philo)
{
	print_philo(philo, THINK_STR, sizeof(THINK_STR));
	usleep(2000);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = arg;
	while (philo->start == 0)
		;
	while (philo->dead == false)
	{
		eat(philo);
		if (philo->dead == true)
			break;
		sleeph(philo);
		if (philo->dead == true)
			break;
		think(philo);
	}
	return (0);
}
