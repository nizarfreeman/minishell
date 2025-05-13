#include "s.h"
static	int	check_atoi(long long res, int s, const char *str)
{
	if (s > 0 && (res > (9223372036854775807LL - (*str - '0')) / 10))
		return (-1);
	if (s < 0 && (-res < (-9223372036854775807LL - 1 + (*str - '0')) / 10))
		return (0);
	return (1);
}

long long	ft_atoi(const char *str)
{
	int			sign;
	long long	res;
	int			h;

	res = 0;
	sign = 1;
	while ((*str >= 9 && *str <= 13) || (*str == ' '))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		h = check_atoi(res, sign, str);
		if (h != 1)
			return (LONG_MAX);
		res = res * 10 + (*str - '0');
		str++;
	}
	return (res * sign);
}