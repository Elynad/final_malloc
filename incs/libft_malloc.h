/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_malloc.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mameyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 15:20:07 by mameyer           #+#    #+#             */
/*   Updated: 2019/05/17 15:20:09 by mameyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_MALLOC_H

# define LIBFT_MALLOC_H

/*
**	INCLUDES
*/

# include <sys/mman.h>
# include <sys/resource.h>
# include <unistd.h>
# include <pthread.h>

/*
**	STRUCTS
*/

typedef struct		s_map
{
	size_t			size;
	size_t			remaining;
	struct s_map	*next;
}					t_map;

typedef struct		s_block
{
	size_t			size;
	struct s_block	*next;
	char			is_free;
	void			*memory;
}					t_block;

typedef struct		s_global
{
	t_map			*tiny;
	t_map			*small;
	t_map			*large;
}					t_global;

/*
** DEFINES
*/

# define TINY_MAX 1024
# define SMALL_MAX (TINY_SIZE * 32)

# define PAGE_SIZE (size_t)getpagesize()
# define MAP_ALIGN(SIZE) (((SIZE - 1) + PAGE_SIZE) - ((SIZE - 1) % PAGE_SIZE))

# define TINY_MMAP_SIZE (MAP_HEADER + ((BLOCK_HEADER + TINY_MAX) * 100))
# define SMALL_MMAP_SIZE (MAP_HEADER + ((BLOCK_HEADER + SMALL_MAX) * 100))

# define PROT PROT_WRITE|PROT_READ
# define MAP MAP_ANONYMOUS|MAP_PRIVATE

/*
**	RETURN DEFINES
*/

# define _SUCCESS_ 0x01
# define _ERROR_ 0x00

/*
**	GLOBAL
*/

extern struct s_global 		g_global;
extern pthread_mutex_t		g_thread_locker;

/*
**	MAIN FUNCTIONS
*/

void				*malloc(size_t size);

/*
**	MAPS
*/

void				*get_from_right_map(size_t requested);
size_t				create_new_map(size_t map_type, t_map **map);

/*
**	BLOCKS
*/

void				*get_free_memory(size_t map_type, t_map *map, size_t size);

/*
**	HELPERS
*/

size_t				align_size(size_t size);

#endif