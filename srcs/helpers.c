/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mameyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 15:36:07 by mameyer           #+#    #+#             */
/*   Updated: 2019/05/17 15:36:08 by mameyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/libft_malloc.h"

/*
**	Param : size_t size : The original size the user needs.
**	Return : size_t size : The size incremented to be aligned.
**
**	Used to align the size the user asked.
*/

size_t		align_size(size_t size)
{
	while (size % 16)
		size++;
	return (size);
}