/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_manip_bis.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:26:06 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/28 16:27:32 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	str_cmp(char *str1, char *str2, int accept_null)
{
	if (!accept_null && (str1 == NULL || str2 == NULL))
	{
		write(2, "err : str_cmp received a NULL string !\n", 40);
		return (FALSE);
	}
	if (str1 == str2)
	{
		return (TRUE);
	}
	if (str1 == NULL || str2 == NULL)
	{
		return (FALSE);
	}
	while (*str1 && (*str1 == *str2))
	{
		str1 ++;
		str2 ++;
	}
	return (*str1 == *str2);
}

int	str_ncmp(char *str1, char *str2, int n, int accept_null)
{
	int	i;

	if (!accept_null && (str1 == NULL || str2 == NULL))
	{
		write(2, "err : str_ncmp received a NULL string !\n", 41);
		return (FALSE);
	}
	if (str1 == str2)
	{
		return (TRUE);
	}
	if (str1 == NULL || str2 == NULL)
	{
		return (FALSE);
	}
	i = 0;
	while (*str1 && (*str1 == *str2) && i < n)
	{
		i ++;
		str1 ++;
		str2 ++;
	}
	return (*str1 == *str2);
}

char	*extract_string(const char *start, int len)
{
	int		i;
	char	*new_str;

	if (start == NULL)
		return (NULL);
	new_str = malloc(sizeof(char) * (len + 1));
	if (new_str == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_str[i] = start[i];
		i ++;
	}
	new_str[i] = '\0';
	return (new_str);
}

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

char	*ft_strdup(const char *s)
{
	int			i;
	size_t		len;
	char		*ns;

	i = 0;
	len = ft_strlen(s);
	if (!s)
	{
		ns = malloc(1);
		if (ns)
			ns[0] = '\0';
		return (ns);
	}
	ns = malloc((len + 1) * sizeof(char));
	if (!ns)
		return (NULL);
	while (s[i])
	{
		ns[i] = s[i];
		i++;
	}
	ns[i] = '\0';
	return (ns);
}
