/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cclaude <cclaude@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 13:07:58 by cclaude           #+#    #+#             */
/*   Updated: 2019/10/16 14:30:43 by cclaude          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strjoin(char const *s1, char const *s2) {
    int len1 = 0, len2 = 0;
    char *new;
    int i, j;

    if (s1 == NULL || s2 == NULL)
        return (NULL);

    // Calculate lengths manually
    while (s1[len1])
        len1++;
    while (s2[len2])
        len2++;

    // Allocate memory
    new = malloc(sizeof(char) * (len1 + len2 + 1));
    if (!new)
        return (NULL);

    // Copy first string
    for (i = 0; i < len1; i++)
        new[i] = s1[i];

    // Copy second string
    for (j = 0; j < len2; j++)
        new[i + j] = s2[j];

    // Null-terminate
    new[len1 + len2] = '\0';

    return (new);
}
