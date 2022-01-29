#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>
#include <assert.h>

/*-pthread option for linker*/
#include <pthread.h>


typedef struct s_philo {
	pthread_t		thread;
	int				id;
	bool			*start;
	bool			dead;
	pthread_mutex_t *death_mtx;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
}	t_philo;

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

typedef struct s_time_info {
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
} t_time_info;

void	usleep_ms(unsigned int time_in_ms)
{
	unsigned long	time;

	time = time_in_ms * 1000;
	assert(time < 1000000);
	usleep(time);
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

long	gettimeofday_in_ms()
{
	struct timeval tv;
	long	time_in_ms;

	gettimeofday(&tv, NULL);
	time_in_ms = tv.tv_sec * 1000;
	time_in_ms += tv.tv_usec / 1000;
	return (time_in_ms);
}

t_time_info	get_time_info(void)
{
	return (set_time_info_once((t_time_info){0, 0, 0}));
}

#include <threads.h>
void	eat(t_philo *philo)
{
	static thread_local int last_time_I_ate;
	long long					time;

	pthread_mutex_lock(philo->left_fork);
	printf("%li\tPhilo %i has taken left fork : %x\n", gettimeofday_in_ms(), philo->id, philo->left_fork);
	pthread_mutex_lock(philo->right_fork);
	printf("%li\tPhilo %i has taken right fork : %x\n", gettimeofday_in_ms(), philo->id, philo->right_fork);
	time = gettimeofday_in_ms();
	printf("%li\tPhilo %i is eating\n", gettimeofday_in_ms(), philo->id);
	if (time - last_time_I_ate > get_time_info().time_to_die)
	{
		philo->dead = true;
		pthread_mutex_lock(philo->death_mtx);
		return ;
	}
	else
		last_time_I_ate = time;
	usleep_ms(get_time_info().time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	printf("%li\tPhilo %i dropped left fork : %x\n", gettimeofday_in_ms(), philo->id, philo->left_fork);
	pthread_mutex_lock(philo->right_fork);
	printf("%li\tPhilo %i dropped right fork : %x\n", gettimeofday_in_ms(), philo->id, philo->right_fork);
}

void	sleeph(t_philo *philo)
{
	printf("%li\tPhilo %i is sleeping\n", gettimeofday_in_ms(),  philo->id);
	usleep_ms(get_time_info().time_to_sleep);
}

void	think(t_philo *philo)
{
	printf("%li\tPhilo %i is thinking\n", gettimeofday_in_ms(),  philo->id);
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
		sleeph(philo);
		think(philo);
	}
	pthread_mutex_lock(philo->death_mtx);
	/*free(philo);*/
	return (0);
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
		if (i % 2 == 0)
			philosophers[i] = init_philo(i, start_even, &mutexes[0],  &mutexes[i], &mutexes[i + 1]);
		else
			philosophers[i] = init_philo(i, start_uneven, &mutexes[0],  &mutexes[i], &mutexes[i + 1]);
		printf("Created philo %i with left fork %hx and right fork %hx\n", i, &mutexes[i], &mutexes[i + 1]);
		i++;
	}
	if (i % 2 == 0)
		philosophers[i] = init_philo(i, start_even, &mutexes[0],  &mutexes[i], &mutexes[1]);
	else
		philosophers[i] = init_philo(i, start_uneven, &mutexes[0],  &mutexes[i], &mutexes[1]);
	printf("Created philo %i with left fork %hx and right fork %hx\n", i, &mutexes[i], &mutexes[1]);
}


int main(int ac, char **av)
{
	t_philo			*philosophers;
	pthread_mutex_t	*mutexes;
	int i;
	int	nb_philo = atoi(av[1]);
	t_time_info	times;
	bool		start_even;
	bool		start_uneven;
	bool		start_last;

	if (ac != 5 && ac != 6)
		return (6);

	nb_philo = atoi(av[1]);

	times =  (t_time_info){
		.time_to_die = atoi(av[2]),
		.time_to_eat = atoi(av[3]),
		.time_to_sleep = atoi(av[4]),
	};

	set_time_info_once(times);

	printf("Time to die set to %s == %i\n", av[2],get_time_info().time_to_die);
	printf("Time to eat set to %s == %i\n", av[3],get_time_info().time_to_eat);
	printf("Time to sleep set to %s == %i\n", av[4],get_time_info().time_to_sleep);

	philosophers = malloc((nb_philo) * sizeof(t_philo));
	if (philosophers == NULL)
		return (-1);

	mutexes = malloc((nb_philo + 1) * sizeof(pthread_mutex_t));
	if (mutexes == NULL)
		return (-1);

	start_even = 0;
	start_uneven = 0;
	init_all_philo(philosophers, nb_philo, mutexes, &start_uneven, &start_even);

	i = 1;
	while (i < nb_philo)
	{
		if (pthread_create(&philosophers[i].thread, NULL, &routine, &philosophers[i]) != 0)
			return (1);
		i++;
	}

	start_even = 1;
	usleep_ms(get_time_info().time_to_eat);
	start_uneven = 1;

	while (1)
	{
		i = 1;
		while (i < nb_philo)
		{
			if (philosophers[i].dead == true)
			{
				printf("%li\tPhilo %i died\n", gettimeofday_in_ms(), philosophers[i].id);
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
