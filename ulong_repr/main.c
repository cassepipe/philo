  static unsigned long    to_the_power_of(unsigned long n, unsigned long p)
  {
      unsigned long result;

      result = 1;
      while (p--)
          result *= n;
      return (result);
  }


  char    *ulong_repr(long n, char *buffer, int *size)
  {
      long            pos;
      unsigned long   i;
      unsigned long   digit;
      unsigned long   a_power_of_ten;

      pos = 1;
      i = n;
      while (i /= 10)
          pos++;
      *size = pos + 1;
      a_power_of_ten = to_the_power_of(10, pos - 1);
      i = 0;
      while (--pos >= 0)
      {
          digit = n / a_power_of_ten;
          buffer[i] = "0123456789"[digit];
          n = n - digit * a_power_of_ten;
          a_power_of_ten /= 10;
		  i++;
      }
      buffer[i] = '\0';
      return (buffer);
  }

#include <unistd.h>
#include <stdio.h>

int main(int ac, char **av)
{
	(void)ac;
	(void)av;

	char buffer[256];
	int size;
	unsigned long num;

	num = 12345678901234567890;
	ulong_repr(num, buffer, &size);
	write(STDOUT_FILENO, buffer, size);
	write(STDOUT_FILENO, "\n", 1);

	printf("%lu\n", sizeof("Hello"));

	return 0;
}
