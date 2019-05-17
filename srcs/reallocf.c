/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reallocf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mameyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 18:44:11 by mameyer           #+#    #+#             */
/*   Updated: 2019/05/17 20:01:12 by mameyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/libft_malloc.h"

/*
**	Param : void *ptr : The pointer we want to realloc.
**			size_t size : The size of the new allocation.
**	Return : A pointer on the new memory.
**
**	The reallocf() function is identical to the realloc() function, except that
**		it will free the passed pointer when the requested memory cannot be
**		allocated.
*/

void		*reallocf(void *ptr, size_t size)
{
	void	*result;

	result = NULL;
	if ((result = realloc(ptr, size)) == NULL)
		free(ptr);
	return (result);
}
