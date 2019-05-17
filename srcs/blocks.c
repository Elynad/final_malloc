/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blocks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mameyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 16:22:32 by mameyer           #+#    #+#             */
/*   Updated: 2019/05/17 16:22:33 by mameyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/libft_malloc.h"

/*
**	Params : size_t map_type : The max size of the map we are looking in, used
**		here when we need to create a new map next to this one.
**			 t_map *map : The begin of the maps we are going to look in.
**			 size_t size : The size the user needs to malloc.
**	Return : A pointer to the free memory we found.
**
**	This function loops through the map given as parameter, and applies
**		check_in_map() function on each link, until the it returns something.
**	If the loop ends and did not returns anything, that means there was no free
**		space inside this page. Then, we call create_new_map() and add it as
**		next map of the last map, and we recursively return the result of
**		get_free_memory() with last->next (that we just created).
*/

void		*get_free_memory(size_t map_type, t_map *map, size_t size)
{
	t_map	*previous;
	size_t	required_size;
	void	*result;

	required_size = size + sizeof(t_block);
	previous = map;
	while (map)
	{
		result = check_in_map(map, required_size, size);
		if (result == NULL)
		{
			previous = map;
			map = map->next;
		}
		else if ((int)result == -1)
			return (NULL);
		else if (result != NULL)
			return (result);
	}
	create_new_map(map_type, &previous->next);
	return (get_free_memory(map_type, previous->next, size));
}

/*
**	Params : t_block *block : The block we need to set.
**			 size_t size : The size of the block.
**	Return : Return the block we just set.
**
**	Update the block size and status, and set its memory pointer.
*/

t_block		*set_block(t_block *block, size_t size)
{
	block->size = size;
	block->next = NULL;
	block->status = USED;
	block->memory = (void *)block + sizeof(t_block);
	return (block);
}

/*
**	Params : t_block *block : The first block of the map we are looking in.
**			 t_map *map : The map which block comes from.
**			 size_t required : The size required for the new block (size
**			 	plus sizeof(t_block)).
**			 size_t size : The size of the new block memory.
**	Return : Return a pointer to the memory of the block in case of success,
**		or NULL / -1 in case of failure.
**
**	Loops through the block to find a block with enough room. If we found a block
**		which is freed and with enough size for our needs, we return it.
**	If there is no block->next, we init this block and the next block, by using
**		set_block, and return its memory.
*/

void		*find_block(t_block *block, t_map *map, size_t required, size_t size)
{
	while (block)
	{
		if (block->status == FREED && block->size >= size)
		{
			block->size == size;
			block->status = USED;
			map->remaining -= required;
			if (block->next)
				keep_memory_optimized(block);
			return (block->memory);
		}
		if (block->next && is_block_in_map(block->next, map) == _ERROR_)
			return ((void *)-1);
		if (block->next == NULL)
		{
			map->remaining -= required;
			block->next = block->ptr + block->size;
			block = block->next;
			set_block(block, size);
			return (block->memory);
		}
		block = block->next;
	}
	return (NULL);
}

/*
**	Param : t_block *block : The block we need to check for optimization.
**
**	This function is only used when we found a block with enough size, and there is a
**		block next to it. We calc the total space, given the gap between our block and
**		its next block.
**	Given this gap, if there is enough room to fit another block with 4 bytes, we align
**		this block and insert it between block and block->next.
*/

void		keep_memory_optimized(t_block *block)
{
	t_block		*new_block;
	size_t		free_size;
	size_t		total_room;

	total_room = (void *)block->next - (void *)block->memory;
	if (total_room - block->size >= sizeof(t_block) + 4)
	{
		new_block = (t_block *)((void *)block->memory + block->size);
		free_size = total_room - sizeof(t_block) - block->size - 1;
		free_size = align_size(free_size);
		if ((void *)new_block + sizeof(t_block) + free_size > (void *)block->next)
			free_size -= 4;
		set_block(new_block, free_size);
		new_block->next = block->next;
		block->next = new_block;
		new_block->status = FREE;
	}
}