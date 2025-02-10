/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 14:08:29 by achamsin          #+#    #+#             */
/*   Updated: 2024/06/11 14:29:43 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	char			*str1;
	char			*str2;
	size_t			i;

	str1 = (char *)s1;
	str2 = (char *)s2;
	i = 0;
	while (i < n)
	{
		if (str1[i] != str2[i])
			return ((unsigned char )str1[i] - (unsigned char )str2[i]);
		i++;
	}
	return (0);
}

// #include <stdio.h>

// int main()
// {
//     char str1[] = "Hello";
//     char str2[] = "World";
//     int result;

//     result = ft_memcmp(str1, str2, 5);
//     printf( "%d", result);
//     return 0;
// }