#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>
#include <assert.h>
#include <pthread.h>
#include <stdlib.h>

#include "t_philo.h"
#include "t_time_info.h"

#include "main.h"

t_philo	init_philo(int id, bool *start, pthread_mutex_t *death_mtx, pthread_mutex_t *left_fork, pthread_mutex_t *right_fork)
{
	t_philo	philo;

	philo.id = id;
	philo.dead = false;
	philo.start = start;
	philo.death_mtx = death_mtx;
	philo.left_fork = left_fork;
	philo.right_fork = right_fork;
	return (philo);
}

void	check_death(bool dead, pthread_mutex_t *death_mtx)
{
	if (dead == true)
		pthread_mutex_lock(death_mtx);
}

void	init_all_philo(t_philo *philosophers, int nb_philo, pthread_mutex_t *mutexes, bool *start_uneven, bool *start_even)
{
	int i;

	i = 0;
	while (i < nb_philo + 1)
	{
		pthread_mutex_init(&mutexes[i], NULL);
		i++;
	}
	i = 1;
	--philosophers;
	while (i < nb_philo)
	{
		// i = 0 <--> philo 1
		if (i % 2 == 0)
			philosophers[i] = init_philo(i, start_uneven, &mutexes[0],  &mutexes[i], &mutexes[i + 1]);
		else
			philosophers[i] = init_philo(i, start_even, &mutexes[0],  &mutexes[i + 1], &mutexes[i]);
		/*printf("Created philo %i with left fork %hx and right fork %hx\n", i, &mutexes[i], &mutexes[i + 1]);*/
		i++;
	}
	if (i % 2 == 0)
		philosophers[i] = init_philo(i, start_uneven, &mutexes[0],  &mutexes[1], &mutexes[i]);
	else
		philosophers[i] = init_philo(i, start_even, &mutexes[0],  &mutexes[i], &mutexes[1]);
	/*printf("Created philo %i with left fork %hx and right fork %hx\n", i, &mutexes[i], &mutexes[1]);*/
}

int main(int ac, char **av)
{
	t_philo			*philosophers;
	pthread_mutex_t	*mutexes;
	int				i;
	int				nb_philo;
	t_time_info		times;
	bool			start_even;
	bool			start_uneven;
	bool			start_last;

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

	mutexes = malloc((nb_philo + 1) * sizeof(pthread_mutex_t));
	if (mutexes == NULL)
		return (-1);

	start_even = 0;
	start_uneven = 0;
	init_all_philo(philosophers, nb_philo, mutexes, &start_uneven, &start_even);

	i = 0;
	while (i < nb_philo)
	{
		if (pthread_create(&philosophers[i].thread, NULL, &routine, &philosophers[i]) != 0)
			return (1);
		/*printf("Launched philosophers[%i] with id = %i\n", i, philosophers[i].id);*/
		i++;
	}

	start_uneven = 1;
	/*usleep(get_time_info().time_to_eat * 1000 - 1000);*/
	start_even = 1;

	while (1)
	{
		i = 1;
		while (i < nb_philo)
		{
			if (philosophers[i].dead == true)
			{
				i = 1;
				while (i < nb_philo)
				{
					pthread_detach(philosophers[i].thread);
					i++;
				}
				return (0);
			}
			i++;
		}
	}

	/*i = 1;*/
	/*while (i < nb_philo)*/
	/*{*/
	/*    if (pthread_join(philosophers[i].thread, NULL) != 0)*/
	/*        return (1);*/
	/*    i++;*/
	/*}*/

	free(philosophers);

	i = 0;
	while (i < nb_philo)
	{
		pthread_mutex_destroy(&mutexes[i]);
		i++;
	}

	free(mutexes);

	return (0);
}
