#include <sys/time.h>
#include <string.h>
#include <unistd.h>

#include "t_philo.h"

#include "print_philo.h"

void	print_philo(const t_philo *philo, const char *action, int act_sz)
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
	write(STDOUT_FILENO, buffer, len);
}
