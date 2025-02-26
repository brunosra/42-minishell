/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 00:23:34 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/01/05 19:10:18 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strncpy(char *dest, const char *src, size_t dest_size) {
    size_t i;

	i = 0;
    if (dest_size == 0)
        return (dest);
    while (i < dest_size - 1 && src[i] != '\0')
	{
        dest[i] = src[i];
		i++;
	}
    dest[i] = '\0';
    return dest;
}

