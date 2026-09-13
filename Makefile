# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: malebrun <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/22 18:14:59 by malebrun          #+#    #+#              #
#    Updated: 2026/06/18 21:11:39 by malebrun         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

C=c++
CFLAGS = -Wall -Werror -Wextra -std=c++17 -g
NAME = scop
SRCS = main.cpp \
		parsing.cpp \
		m4.cpp \
		camera.cpp \
		vulkan.cpp \
		swapchain.cpp \
		VkRender.cpp \
		inputmanager.cpp \
		material.cpp \
		textures.cpp
OBJ_DIR = .build/
OBJS = $(SRCS:%.cpp=$(OBJ_DIR)%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(C) $(CFLAGS) -lvulkan -lglfw $(OBJS) -o $(NAME)

$(OBJ_DIR)%.o:%.cpp
	@mkdir -p $(dir $@)
	$(C) $(CFLAGS) $< -c -o $@

fclean: clean
	rm -f $(NAME)

clean:
	rm -rf $(OBJ_DIR)

re: fclean all

.PHONY: all fclean clean re
