/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achamsin <achamsin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 16:42:56 by achamsin          #+#    #+#             */
/*   Updated: 2024/06/12 16:47:37 by achamsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

void	ft_putendl_fd(char *s, int fd)
{
	if (!s)
		return ;
	ft_putstr_fd(s, fd);
	write(fd, "\n", 1);
}

// #include <stdio.h>

// int main()
// {
// 	int fd = 1; // Assuming 1 for standard output (stdout)
// 	char *str = "Hello, World!";

// 	ft_putendl_fd(str, fd);

// 	return 0;
// }