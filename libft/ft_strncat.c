/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 19:04:22 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/01/06 11:45:26 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strncat(char *dest, const char *src, size_t dest_size) {
    
	size_t dest_len;
    size_t i;

	i = 0;
	dest_len = ft_strlen(dest);
    if (dest_len >= dest_size - 1)
        return (dest);
    while (i < dest_size - dest_len - 1 && src[i] != '\0')
	{
        dest[dest_len + i] = src[i];
		i++;
	}
    dest[dest_len + i] = '\0';
    return (dest);
}
