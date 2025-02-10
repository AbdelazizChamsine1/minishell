/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:55:46 by achamsin          #+#    #+#             */
/*   Updated: 2024/06/20 10:39:33 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*str;

	str = s;
	i = 0;
	while (i < n)
	{
		str[i] = 0;
		i++;
	}
}

// #include <stdio.h>

// int main()
// {
//     char str[30] = "ABCD EFGH";
//     printf("Before bzero: %s\n", str);

//     ft_bzero(str, 2);
//     printf("After bzero: %s\n", str);

//     return 0;
// }