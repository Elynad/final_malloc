/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mameyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 18:19:43 by mameyer           #+#    #+#             */
/*   Updated: 2019/05/17 18:19:51 by mameyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/libft_malloc.h"

/*
**	Param : void *ptr : The pointer we need to realloc.
**			t_map *map : The map we are looking in.
**			size_t size : The size of the new allocation.
**	Return : A pointer, new or old, or ERROR.
**
**	This function will loop through the map we got in paramater. If the pointer
**		really is in this map (is_pointer_valid() is used to check that), we
**		create a pointer on the parent block of the memory. With this block,
**		if its next block is free and both block and block->next have enough
**		size to hold the new allocation, we merge the two blocks and return the
**		first one.
**		In the other case, we call create_new_pointer().
*/

void		*realloc_helper(void *ptr, t_map *map, size_t size)
{
	t_map		*prev;
	t_block		*block;

	prev = NULL;
	while (map)
	{
		if (is_pointer_valid(ptr, map) == _SUCCESS_)
		{
			block = (t_block *)(ptr - sizeof(t_block));
			if (block->next && block->next->is_free == FREE
				&& block->next->size + sizeof(t_block) > size)
			{
				block->next = block->next->next;
				block->size = block->size + block->next->size + sizeof(t_block);
				return (ptr);
			}
			else
				return (create_new_pointer(block, ptr, size));
		}
		prev = map;
		map = map->next;
	}
	return (_ERROR_);
}

/*
**	Param : t_block *block : The original block of the memory we want to realloc.
**			void *ptr : The pointer on the memory we want to realloc.
**			size_t size : The size of the new allocation.
**	Return : The newly allocated memory pointer.
**
**	Call malloc with the new size to create a new block of memory, then call
**		to ft_memmove() to copy the data from the old pointer to the new one. Then,
**		free the old pointer and returns the new one.
*/

void		*create_new_pointer(t_block *block, void *ptr, size_t size)
{
	void		*new_ptr;

	if (!(new_ptr = malloc(size)))
		return (NULL);
	new_ptr = ft_memmove(new_ptr, ptr, block->size);
	free(ptr);
	return (new_ptr);
}