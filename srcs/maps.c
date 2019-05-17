/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maps.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mameyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 15:45:37 by mameyer           #+#    #+#             */
/*   Updated: 2019/05/17 19:58:57 by mameyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/libft_malloc.h"

/*
**	Param : size_t requested : The aligned size the user needs.
**	Return : void *result : A pointer to the memory we allocated (or NULL if
**		it failed).
**
**	Given the size the user needs, we try to get a free memory block in the
**		right map (tiny, small or large). Before trying to assign the result
**		pointer, we check if the map we are going to use exists, and if it does
**		not, we check if the map creating was successful.
*/

void		*get_from_right_map(size_t requested)
{
	void	*result;

	result = NULL;
	if (requested <= TINY_MAX)
	{
		if (!g_global.tiny && !create_new_map(TINY_MAX, &g_global.tiny))
			return (NULL);
		result = get_free_memory(TINY_MAX, g_global.tiny, requested);
	}
	else if (requested <= SMALL_MAX)
	{
		if (!g_global.small && !create_new_map(SMALL_MAX, &g_global.small))
			return (NULL);
		result = get_free_memory(SMALL_MAX, g_global.small, requested);
	}
	else
	{
		if (!g_global.large && !create_new_map(requested, &g_global.large))
			return (NULL);
		result = get_free_memory(requested, g_global.large, requested);
	}
	return (result);
}

/*
**	Param : size_t map_type : The max size of the type of map we need, used as
**		a type for conditions.
**			t_map **map :	A pointer to the t_map* we need to mmap.
**	Return : size_t : A typedef of SUCCESS or FAILURE.
**
**	Will mmap a new map and assign it to the t_map** we gave as parameter. The
**		size of mmap is of course aligned.
**	Returns ERROR of SUCCESS.
*/

size_t		create_new_map(size_t map_type, t_map **map)
{
	size_t		mmap_size;

	if (map_type == TINY_MAX)
		mmap_size = MAP_ALIGN(TINY_MMAP_SIZE);
	else if (map_type == SMALL_MAX)
		mmap_size = MAP_ALIGN(SMALL_MMAP_SIZE);
	else
	{
		mmap_size = map_type + sizeof(t_map) + sizeof(t_block) + 1;
		mmap_size = MAP_ALIGN(mmap_size);
	}
	*map = mmap(NULL, mmap_size, PROT, MAP, -1, 0);
	if (*map == MAP_FAILED)
		return (_ERROR_);
	(*map)->size = mmap_size;
	(*map)->remaining = mmap_size - sizeof(t_map);
	(*map)->next = NULL;
	return (_SUCCESS_);
}

/*
**	Param : t_map *map : The link of map we are checking.
**			size_t required : The total size we need to create a node (size and
**				sizeof(t_block)).
**			int size : The aligned size the user requested. Pretty useless, as
**				we can retrieve it by a simple calcs by doing :
**				required - sizeof(t_block)
**	Return : Will return a pointer, NULL, or -1, given the success of this
**		function.
**
**	This function check if we can set a new block inside the map given as
**		parameter. If there is not enough room, we return NULL.
**	We assign to block the address of the first block contained inside the map.
**		If this block is freed and has 0 as size, that means we just created
**		this map and block, so we set this block and return its memory pointer.
**	If the first block is not freed or does not have 0 as size, we call
**		find_block() and return something given its result.
*/

void		*check_in_map(t_map *map, size_t required, int size)
{
	void		*result;
	t_block		*block;

	if (map->remaining > required)
	{
		block = (void *)map + sizeof(t_map);
		if (block->is_free == FREE && block->size == 0)
		{
			set_block(block, size);
			map->remaining -= required;
			return (block->memory);
		}
		result = find_block(block, map, required, size);
		if ((int)result == -1)
			return (void *)-1;
		else if (result != NULL)
			return (result);
	}
	return (NULL);
}
