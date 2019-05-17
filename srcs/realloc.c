/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mameyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 18:17:18 by mameyer           #+#    #+#             */
/*   Updated: 2019/05/17 18:17:20 by mameyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/libft_malloc.h"

/*
**	Params : void *ptr : The pointer we want to realloc.
**			 size_t size : The size of the new allocation.
**	Return : A pointer on the realloced memory.
**
**	Realloc function tries to change the size of the allocation pointed to by
**		ptr to size, and returns ptr. If there is not enough room to enlarge the
**		memory allocation pointed to by ptr, realloc creates a new allocation,
**		(using malloc()), copies as much of the old data as will fit to the
**		new allocation, frees the old allocation, and returns a pointer on the
**		allocated memory.
**	If ptr is NULL, realloc is identical to a call to malloc.
*/

void		*realloc(void *ptr, size_t size)
{
	void	*result;

	pthread_mutex_lock(&g_thread_locker);
	if (ptr == NULL)
		return (malloc(size));
	if (ptr == NULL || size == 0)
		return (NULL);
	if ((result = realloc_helper(ptr, g_global.tiny, size)) != _ERROR_)
		return (result);
	if ((result = realloc_helper(ptr, g_global.small, size)) != _ERROR_)
		return (result);
	if ((result = realloc_helper(ptr, g_global.large, size)) != _ERROR_)
		return (result);
	if (!size)
	{
		free (ptr);
		return (malloc(size));
	}
	pthread_mutex_unlock(&g_thread_locker);
	return (NULL);
}