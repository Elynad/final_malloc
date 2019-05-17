/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maps.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mameyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 15:45:37 by mameyer           #+#    #+#             */
/*   Updated: 2019/05/17 15:45:38 by mameyer          ###   ########.fr       */
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
	if (size <= TINY_MAX)
	{
		if (!g_global.tiny && !create_new_map(TINY_MAX, &g_global.tiny))
			return (NULL);
		ptr = get_free_memory(TINY_MAX, g_global.tiny, size);
	}
	else if (size <= SMALL_MAX)
	{
		if (!g_global.small && !create_new_map(SMALL_MAX, &g_global.small))
			return (NULL);
		ptr = get_free_memory(SMALL_MAX, g_global.small, size);
	}
	else
	{
		if (!g_global.large && !create_new_map(size, &g_global.large))
			return (NULL);
		ptr = get_free_memory(size, g_global.large, size);
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
	if (*map = MAP_FAILED)
		return (_ERROR_);
	(*map)->size = mmap_size;
	(*map)->remaining = mmap_size - sizeof(t_map);
	(*map)->next = NULL;
	return (_SUCCESS_);
}