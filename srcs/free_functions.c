/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mameyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 17:46:59 by mameyer           #+#    #+#             */
/*   Updated: 2019/05/17 19:58:09 by mameyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/libft_malloc.h"

/*
**	Params : void *ptr : The pointer we want to free.
**			 t_map *map : The map we are looking in.
**	Return : SUCCESS or ERROR.
**
**	This function will loop through the map given as parameter,
**		check if the pointer is in this map. If it does, we free
**		it and update size.
**	If check_is_unmap_required() fails, we call merge_free_blocks().
**		We always keep previous for this purpose.
*/

int			find_memory_in_map(void *ptr, t_map *map)
{
	t_map		*previous;
	t_block		*block;

	previous = NULL;
	while (map)
	{
		if (is_pointer_valid(ptr, map) == _SUCCESS_)
		{
			ptr = ptr - sizeof(t_block);
			block = (t_block *)ptr;
			block->is_free = FREE;
			map->remaining += sizeof(t_block) + block->size;
			if (!check_is_unmap_required(map, previous))
				merge_freed_blocks(map);
			return (_SUCCESS_);
		}
		previous = map;
		map = map->next;
	}
	return (_ERROR_);
}

/*
**	Params : t_map *map : The map we need to check.
**			 t_map *prev : The previous map, used when we unmap map.
**	Return : SUCCESS or ERROR.
**
**	Will check inside of map if every block is free. If all blocks are freed,
**		then we can unmap this map and set global variable as prev.
*/

int			check_is_unmap_required(t_map *map, t_map *prev)
{
	t_block		*block;

	if (!map->next)
		return (_ERROR_);
	block = (void *)map + sizeof(t_map);
	while (block)
	{
		if (block->is_free == USED)
			return (_ERROR_);
		block = block->next;
	}
	if (g_global.tiny == map)
		g_global.tiny = map->next;
	if (g_global.small == map)
		g_global.small = map->next;
	if (g_global.large == map)
		g_global.large = map->next;
	if (prev != NULL)
		prev->next = map->next;
	munmap(map, map->size);
	return (_SUCCESS_);
}

/*
**	Params : t_map *map : The map we need to check for adjacents freed blocks.
**
**	Called when check_if_unmap_required() fails.
**	Loops through the blocks contained in map, and check if this block is
**		free and the block next to it is free too. If these conditions are
**		true, will merge block with block->next.
*/

void		merge_freed_blocks(t_map *map)
{
	t_block		*block;

	block = (void *)map + sizeof(t_map);
	while (block->next)
	{
		if (block->is_free == FREE && block->next->is_free == FREE)
		{
			block->size += block->next->size + sizeof(t_block);
			block->next = block->next->next;
			merge_freed_blocks(map);
			return ;
		}
		block = block->next;
	}
}
