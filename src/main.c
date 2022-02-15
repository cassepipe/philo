#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>
#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "t_philo.h"
#include "t_time_info.h"
#include "defines.h"

#include "main.h"

t_philo	init_philo(int id, bool *start, pthread_mutex_t *death_mtx, t_fork *left_fork, t_fork *right_fork)
{
	t_philo	philo;

	philo.id = id;
	philo.dead = false;
	philo.start = start;
	philo.death_mtx = death_mtx;
	philo.left_fork  = left_fork;
	philo.right_fork = right_fork;
	return (philo);
}

void	init_forks(t_fork *forks, int nb_philo)
{
	int i;

	i = 0;
	while (i < nb_philo + 1)
	{
		forks[i].fork_status = UNLOCKED;
		pthread_mutex_init(&(forks[i].fork_mtx), NULL);
		i++;
	}
}

void	init_all_philo(t_philo *philosophers, int nb_philo, pthread_mutex_t *death_mutex, t_fork *forks, bool *start)
{
	int i;

	i = 1;
	--philosophers;
	while (i < nb_philo)
	{
		if (i % 2 == 0)
			philosophers[i] = init_philo(i, &start[0], death_mutex,  &forks[i], &forks[i + 1]);
		else
			philosophers[i] = init_philo(i, &start[1], death_mutex,  &forks[i + 1], &forks[i]);
		printf("Created philo %i with left fork %hx and right fork %hx\n", i, &forks[i], &forks[i + 1]);
		i++;
	}
	if (i % 2 == 0)
		philosophers[i] = init_philo(i, &start[0], death_mutex,  &forks[1], &forks[i]);
	else
		philosophers[i] = init_philo(i, &start[1], death_mutex,  &forks[i], &forks[1]);
	printf("Created philo %i with left fork %hx and right fork %hx\n", i, &forks[i], &forks[1]);
}

void	loop(t_philo *philosophers, int nb_philo)
{
	int i;

	i = 0;
	while (1)
	{
		i = 0;
		while (i < nb_philo)
		{
			if (philosophers[i].dead == true)
			{
				return;
			}
			i++;
		}
	}
}

int main(int ac, char **av)
{
	t_philo			*philosophers;
	pthread_mutex_t	*death_mtx;
	int				i;
	int				nb_philo;
	t_time_info		times;
	bool			start[2];
	t_fork			*forks;

	if (ac != 5 && ac != 6)
		return (6);

	nb_philo = atoi(av[1]);

	times =  (t_time_info){
		.time_to_die = atoi(av[2]),
		.time_to_eat = atoi(av[3]),
		.time_to_sleep = atoi(av[4]),
	};

	set_time_info_once(times);

	philosophers = malloc((nb_philo) * sizeof(t_philo));
	if (philosophers == NULL)
		return (-1);

	death_mtx = malloc(sizeof(pthread_mutex_t));
	if (death_mtx == NULL)
		return (-1);

	forks = malloc((nb_philo + 1) * sizeof(t_fork));
	if (forks == NULL)
		return (-1);

	init_forks(forks, nb_philo);

	start[0] = 0;
	start[1] = 0;
	init_all_philo(philosophers, nb_philo, death_mtx, forks, start);

	i = 0;
	while (i < nb_philo)
	{
		if (pthread_create(&philosophers[i].thread, NULL, &routine, &philosophers[i]) != 0)
			return (1);
		/*printf("Launched philosophers[%i] with id = %i\n", i, philosophers[i].id);*/
		i++;
	}

	start[0] = 1;
	/*usleep(get_time_info().time_to_eat * 1000 - 1000);*/
	usleep(10000);
	start[1] = 1;

	loop(philosophers, nb_philo);

	i = 0;
	while (i < nb_philo)
	{
		philosophers[i].dead =  true;
		i++;
	}
	i = 0;
	while (i < nb_philo)
	{
		pthread_join(philosophers[i].thread, NULL);
		/*pthread_detach(philosophers[i].thread);*/
		i++;
	}

	free(philosophers);

	i = 0;
	while (i < nb_philo)
	{
		pthread_mutex_destroy(&forks[i].fork_mtx);
		i++;
	}

	free(forks);
	free(death_mtx);

	return (0);
}
