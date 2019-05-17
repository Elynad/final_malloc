/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mameyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 17:37:52 by mameyer           #+#    #+#             */
/*   Updated: 2019/05/17 19:55:29 by mameyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/libft_malloc.h"

/*
**	Param : void *ptr : The pointer we want to free.
**
**	Free a pointer.
**	Protect from multi-threading.
**	Check if the pointer is not NULL, then call find_memory_in_map() on each
**		map of the g_global variable.
*/

void		free(void *ptr)
{
	pthread_mutex_lock(&g_thread_locker);
	if (ptr == NULL)
		return ;
	if (find_memory_in_map(ptr, g_global.tiny) == _SUCCESS_)
		return ;
	if (find_memory_in_map(ptr, g_global.small) == _SUCCESS_)
		return ;
	if (find_memory_in_map(ptr, g_global.large) == _SUCCESS_)
		return ;
	pthread_mutex_unlock(&g_thread_locker);
}
