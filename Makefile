# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tpouget <cassepipe@ymail.com>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/05/23 16:37:33 by tpouget           #+#    #+#              #
#    Updated: 2021/09/15 14:36:55 by bajaba           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#################
##  VARIABLES  ##
#################

NAME			=	philo

#CC	  		  	=	gcc-11
CC	  		  	=	clang-12

#Add -Werror before correction
CFLAGS			=	-Wall -Wextra  -Iinc -g3 -Wno-format

SANITIZER		=	#-fsanitize=address

ANALYSER		=	#-fanalyzer

SOURCES			=  $(wildcard src/*.c)	

LIBS			=  -pthread

INC/HEADERS		=	

OBJ/OBJECTS		=	$(patsubst src/%.c, obj/%.o, $(SOURCES))
	

#	Rules

all:			$(NAME)

$(NAME):		${OBJ/OBJECTS}
				@echo "Linking..."
				${CC} ${SANITIZER} ${ANALYSER} ${LDFLAGS} ${OBJ/OBJECTS} ${LIBS} -o $@ 

obj/%.o:		src/%.c	${INC/HEADERS} Makefile | obj
				${CC} ${CFLAGS} ${ANALYSER} -c $< -o $@
obj:			
				mkdir obj

clean:			
				rm -rf obj

fclean:			clean
				rm -rf $(NAME)

re:				fclean all

.PHONY:			all clean fclean re test 
