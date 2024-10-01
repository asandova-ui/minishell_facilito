/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 02:29:18 by jamorale          #+#    #+#             */
/*   Updated: 2024/10/01 09:31:50 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../printf/includes/ft_printf.h"
#include "../../../printf/libft/libft.h"

char	*ft_strtok(char *str, const char *delim)
{
	static char	*last_token;
	char		*token_start;

	last_token = NULL;
	if (str == NULL)
		str = last_token;
	if (str == NULL || *str == '\0')
		return (NULL);
	str += ft_strspn(str, delim);
	if (*str == '\0')
		return (NULL);
	token_start = str;
	str = ft_strpbrk(str, delim);
	if (str == NULL)
		last_token = NULL;
	else
	{
		*str = '\0';
		last_token = str + 1;
	}
	return (token_start);
}

char	*ft_strtok_r(char *str, const char *delim, char **saveptr)
{
	char	*token_start;

	if (str == NULL)
		str = *saveptr;
	if (str == NULL || *str == '\0')
	{
		*saveptr = NULL;
		return (NULL);
	}
	str += ft_strspn(str, delim);
	if (*str == '\0')
	{
		*saveptr = NULL;
		return (NULL);
	}
	token_start = str;
	str = ft_strpbrk(str, delim);
	if (str == NULL)
		*saveptr = NULL;
	else
	{
		*str = '\0';
		*saveptr = str + 1;
	}
	return (token_start);
}

size_t	ft_strspn(const char *str, const char *accept)
{
	const char	*s;
	const char	*a;

	s = str;
	while (*s)
	{
		a = accept;
		while (*a)
		{
			if (*s == *a)
				break ;
			a++;
		}
		if (*a == '\0')
			return ((size_t)(s - str));
		s++;
	}
	return ((size_t)(s - str));
}

char	*ft_strpbrk(const char *str, const char *accept)
{
	const char	*a;

	while (*str != '\0')
	{
		a = accept;
		while (*a != '\0')
		{
			if (*str == *a)
				return ((char *)str);
			a++;
		}
		str++;
	}
	return (NULL);
}

char *ft_strstr(const char *haystack, const char *needle)
{
    const char *h = haystack;
    const char *n = needle;
    
    if (*needle == '\0')
        return ((char *)haystack);
    while (*haystack != '\0')
    {
        h = haystack;
        n = needle;
        while (*h != '\0' && *n != '\0' && *h == *n)
        {
            h++;
            n++;
        }
        if (*n == '\0')
            return ((char *)haystack);
        haystack++;
    }
    return (NULL);
}
