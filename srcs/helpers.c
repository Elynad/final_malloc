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

/*
**	Params : t_block *block : The block we need to check.
**			 t_map *map : The map we are looking in.
**	Return : A define of SUCCESS or ERROR as int.
**
**	Used to check if the block is inside the map. For this purpose
**		we compare the addresses.
*/

int			is_block_in_map(t_block *block, t_map *map)
{
	if ((void *)block > (void *)map
		&& (unsigned int)block < (unsigned int)map + map->size)
		return (_SUCCESS_);
	else
		return (_ERROR_);
}

/*
**	Params : void *ptr : The pointer we need to free.
**			 t_map *map : The map we are looking in.
**	Return : SUCCESS or ERROR.
**
**	This function checks if ptr is inside map and is valid. It is useful
**		when we call free or realloc, and we want to check if the pointer is
**		valid (means that we allocated this ourselves).
*/

int			is_pointer_valid(void *ptr, t_map *map)
{
	t_block		*block;

	if (ptr > (void *)map && (unsigned int)ptr < (unsigned int)map + map->size)
	{
		block = (void *)map + sizeof(t_map);
		while (block)
		{
			if (block->memory == ptr)
				return (_SUCCESS_);
			block = block->next;
		}
	}
	return (_ERROR_);
}