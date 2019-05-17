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
# define SMALL_MAX (TINY_MAX * 32)

# define PAGE_SIZE (size_t)getpagesize()
# define MAP_ALIGN(SIZE) (((SIZE - 1) + PAGE_SIZE) - ((SIZE - 1) % PAGE_SIZE))

# define MAP_HEADER sizeof(t_map)
# define BLOCK_HEADER sizeof(t_block)

# define TINY_MMAP_SIZE (MAP_HEADER + ((BLOCK_HEADER + TINY_MAX) * 100))
# define SMALL_MMAP_SIZE (MAP_HEADER + ((BLOCK_HEADER + SMALL_MAX) * 100))

# define PROT PROT_WRITE|PROT_READ
# define MAP MAP_ANONYMOUS|MAP_PRIVATE

# define FREE 'y'
# define USED 'n'

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
**	============================================================================
**	|									MALLOC								   |
**	============================================================================
*/

void				*malloc(size_t size);

/*
**	MAPS
*/

void				*get_from_right_map(size_t requested);
size_t				create_new_map(size_t map_type, t_map **map);
void				*check_in_map(t_map *map, size_t required, int size);

/*
**	BLOCKS
*/

void				*get_free_memory(size_t map_type, t_map *map, size_t size);
t_block				*set_block(t_block *block, size_t size);
void				*find_block(t_block *block, t_map *map, size_t required, size_t size);
void				keep_memory_optimized(t_block *block);

/*
**	HELPERS
*/

size_t				align_size(size_t size);
int					is_block_in_map(t_block *block, t_map *map);
int					is_pointer_valid(void *ptr, t_map *map);
void				*ft_memmove(void *dest, const void *src, size_t len);
void				*ft_memcpy(void *dest, const void *src, size_t len);

/*
**	============================================================================
**	|								 	 FREE 								   |
**	============================================================================
*/

void				free(void *ptr);

int					find_memory_in_map(void *ptr, t_map *map);
int					check_is_unmap_required(t_map *map, t_map *prev);
void				merge_freed_blocks(t_map *map);

/*
**	============================================================================
**	|								  REALLOC								   |
**	============================================================================
*/

void				*realloc(void *ptr, size_t size);

void				*realloc_helper(void *ptr, t_map *map, size_t size);
void				*create_new_pointer(t_block *block, void *ptr, size_t size);

/*
**	============================================================================
**	|								  REALLOCF								   |
**	============================================================================
*/

void				*reallocf(void *ptr, size_t size);

#endif