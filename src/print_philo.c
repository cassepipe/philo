#include <sys/time.h>
#include <string.h>
#include <unistd.h>

#include "t_philo.h"
#include "defines.h"

#include "print_philo.h"

void	print_philo(const t_philo *philo, const char *action, int act_sz)
{
	char	buffer[256];
	int		len;

	if (philo->dead == false)
	{
		len = 0;
		ulong_repr(gettimeofday_in_ms(), buffer, &len);
		memcpy(buffer + len, " Philo ", sizeof(" Philo ") - 1);
		len += sizeof(" Philo ") - 1;
		ulong_repr(philo->id, buffer + len, &len);
		memcpy(buffer + len, action, act_sz - 1);
		len += act_sz - 1;
		pthread_mutex_lock(philo->death_mtx);
		if (philo->dead == false)
			write(STDOUT_FILENO, buffer, len);
		pthread_mutex_unlock(philo->death_mtx);
	}
}

void	print_dead_philo(const t_philo *philo, const char *action, int act_sz)
{
	char	buffer[256];
	int		len;

	len = 0;
	ulong_repr(gettimeofday_in_ms(), buffer, &len);
	memcpy(buffer + len, " Philo ", sizeof(" Philo ") - 1);
	len += sizeof(" Philo ") - 1;
	ulong_repr(philo->id, buffer + len, &len);
	memcpy(buffer + len, action, act_sz - 1);
	len += act_sz - 1;
	pthread_mutex_lock(philo->death_mtx);
	write(STDOUT_FILENO, buffer, len);
	pthread_mutex_unlock(philo->death_mtx);
}

void	print_philo_death(const t_philo *philo)
{
	char	buffer[256];
	int		len;

	len = 0;
	ulong_repr(gettimeofday_in_ms(), buffer, &len);
	memcpy(buffer + len, " Philo ", sizeof(" Philo ") - 1);
	len += sizeof(" Philo ") - 1;
	ulong_repr(philo->id, buffer + len, &len);
	memcpy(buffer + len, DIE_STR, sizeof(DIE_STR) - 1);
	len += sizeof(DIE_STR) - 1;
	pthread_mutex_lock(philo->death_mtx);
	write(STDOUT_FILENO, buffer, len);
	pthread_mutex_unlock(philo->death_mtx);
}

