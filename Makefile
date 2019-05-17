# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cchameyr <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/10/21 12:14:25 by cchameyr          #+#    #+#              #
#    Updated: 2019/05/16 14:00:08 by mameyer          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

FILES =				malloc.c \
					helpers.c \
					maps.c \
					blocks.c \
					free.c \
					free_functions.c \
					realloc.c \
					realloc_functions.c \
					reallocf.c


SRCS =				$(addprefix srcs/, $(FILES))

OBJS =				$(addprefix objs/, $(FILES:.c=.o))

CC =				gcc

NAME =				libft_malloc_$(HOSTTYPE).so

NAME_FINAL =		libft_malloc.so

FSNAITIZE =			-g3 -fsanitize=address

FLAGS =				-Wall -Wextra -Werror #$(FSNAITIZE)

RM =				rm -rf

all: $(NAME_FINAL)

$(NAME_FINAL): $(OBJS)
	$(CC) -shared $(FLAGS) $(OBJS) -o $(NAME)
	ln -s $(NAME) libft_malloc.so

$(OBJS):
	$(CC) $(FLAGS) -c $(SRCS)
	@make objs_mv

objs_mv:
	@mkdir objs
	@mv $(FILES:.c=.o) ./objs/

objs_rm:
	@$(RM) objs
	@$(RM) $(FILES:.c=.o)

clean: objs_rm

fclean: clean
	$(RM) $(NAME) $(NAME_FINAL)

re: fclean all
