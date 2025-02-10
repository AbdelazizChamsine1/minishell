/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 13:56:03 by achamsin          #+#    #+#             */
/*   Updated: 2024/06/12 14:55:19 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	int	i;

	if (!s)
		return ;
	i = 0;
	while (s[i] != '\0')
	{
		(*f)(i, &s[i]);
		i++;
	}
	s[i] = '\0';
}

// #include <stdio.h>
// #include "libft.h"

// void increment_ascii(unsigned int i, char *c)
// {
//     *c = *c + i;
// }

// int main(void)
// {
//     char str[] = "hello";

//     printf("Original string: %s\n", str);
//     ft_striteri(str, increment_ascii);
//     printf("Modified string: %s\n", str);

//     return 0;
// }
