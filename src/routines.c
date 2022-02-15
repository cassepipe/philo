#include <threads.h>
#include <unistd.h>

#include "t_philo.h"
#include "t_fork.h"

#include "defines.h"
#include "routines.h"

bool	take_fork(t_philo *philo, t_fork *fork)
{
	pthread_mutex_lock(&fork->fork_mtx);
	if (fork->fork_status == UNLOCKED)
	{
		fork->fork_status = LOCKED;
		print_philo(philo, FORK_STR, sizeof(FORK_STR));
		pthread_mutex_unlock(&fork->fork_mtx);
		return (true);
	}
	pthread_mutex_unlock(&fork->fork_mtx);
	return false;
}

void	put_down_fork(t_fork *fork)
{
		pthread_mutex_lock(&fork->fork_mtx);
		fork->fork_status = UNLOCKED;
		pthread_mutex_unlock(&fork->fork_mtx);
}

void	put_down_both_forks(t_fork *fork1, t_fork *fork2)
{
	put_down_fork(fork1);
	put_down_fork(fork2);
}

void	eat(t_philo *philo)
{
	static thread_local long	last_time_I_ate;
	long 						time;
	long						diff;

	while (1)
	{
		if (take_fork(philo, philo->left_fork) == false)
		{
			usleep(500);
			continue;
		}
		if (take_fork(philo, philo->right_fork) == false)
		{
			put_down_fork(philo->left_fork);
			usleep(500);
			continue;
		}
		break;
	}

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
	put_down_both_forks(philo->left_fork, philo->right_fork);
	/*printf("%li\tPhilo %i dropped left fork : %hx\n", gettimeofday_in_ms(), philo->id,  philo->left_fork);*/
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
	usleep(500);
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
