/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:58:31 by achamsin          #+#    #+#             */
/*   Updated: 2024/06/11 11:58:33 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	if (!s)
		return (NULL);
	i = 0;
	while (i < n)
	{
		ptr[i++] = (unsigned char)c;
	}
	return (s);
}

// #include <stdio.h>

// int main()
// {
//     char str[30] = "ABCD EFGH";
//     printf("Before memset: %s\n", str);

//     ft_memset(str, 'x', 2);
//     printf("After memset: %s\n", str);

//     return 0;
// }
