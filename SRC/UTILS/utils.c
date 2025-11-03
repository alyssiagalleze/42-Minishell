/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfiette <tfiette@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:32:47 by tfiette           #+#    #+#             */
/*   Updated: 2025/11/03 14:26:52 by tfiette          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_allocate_str(long i, long *size)
{
	char	*str;

	*size = 0;
	if (i <= 0)
	{
		*size = *size + 1;
		i = i * -1;
	}
	while (i)
	{
		*size = *size + 1;
		i = i / 10;
	}
	str = malloc(sizeof(char) * (*size) + 1);
	return (str);
}

char	*ft_itoa(int n)
{
	long	i;
	long	size;
	char	*str;

	i = n;
	str = ft_allocate_str(i, &size);
	if (str == NULL)
		return (NULL);
	if (i == 0)
		str[0] = '0';
	if (i < 0)
	{
		str[0] = '-';
		i = i * -1;
	}
	str[size] = '\0';
	size --;
	while (i >= 1)
	{
		str[size] = (i % 10) + '0';
		size --;
		i = i / 10;
	}
	return (str);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sign;
	int	res;

	res = 0;
	sign = 1;
	i = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || (nptr[i] == ' '))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
		res = res * 10 + nptr[i++] - '0';
	return (sign * res);
}

void	*ft_free(void *ptr)
{
	if (ptr)
		free(ptr);
	return (NULL);
}

void	ft_close(int *fd)
{
	if (*fd != -1)
		close(*fd);
	*fd = -1;
}
