/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mameyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 15:20:58 by mameyer           #+#    #+#             */
/*   Updated: 2019/05/17 15:21:00 by mameyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/libft_malloc.h"

/*
**	Initalizing global variables.
**	g_global will contain all maps (tiny, small and large)
**	g_thread_locker is used to protect malloc against multi-threading.
*/

t_global			g_global = {NULL, NULL, NULL};
pthread_mutex_t		g_thread_locker;

/*
**	Param : size_t size : The size of the memory the user want to allocate.
**	Return : void *result : A pointer to the begin of the memory we just
**		allocated.
**
**	First, we align the size of the memory the user asked for, by using
**		align_size() function. Then, we call get_right_map() with this size.
**	Note that we use pthread library functions to protect this malloc against
**		multi-threading.
*/

void		*malloc(size_t size)
{
	void	*result;

	size = align_size(size);
	pthread_mutex_lock(&g_thread_locker);
	result = get_from_right_map(size);
	pthread_mutex_unlock(&g_thread_locker);
	return (result);
}