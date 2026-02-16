# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mgarnier <mgarnier@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/02/12 15:54:22 by mgarnier          #+#    #+#              #
#    Updated: 2026/02/16 15:56:19 by mgarnier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC			= clang
CFLAGS		= -Wall -Wextra -Werror -pthread -fsanitize=thread -g

# COLOR
GREEN   := \033[1;38;5;46m
RESET   := \033[0m

# DIR
DIR			= sources/
DIR_ERROR	= sources/error_management/
DIR_PARSE	= sources/parsing/
OBJ_DIR		= objects

# MINISHELL
NAME		= philo
HEADER		= -Iincludes

SRC			= $(DIR)main.c \
				$(DIR)structure.c \
				$(DIR)routine.c \
				$(DIR_ERROR)error.c \
				$(DIR_PARSE)utils.c \
				$(DIR_PARSE)parse_args.c

OBJ			= $(SRC:%.c=$(OBJ_DIR)/%.o)

all :		$(NAME)

$(NAME): $(OBJ)
			@$(CC) $(CFLAGS) $(HEADER) $(OBJ) -o $(NAME)
			@printf "${GREEN}\r[▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬] SUCCESS 100%%${RESET}\n"

TOTAL		:= $(words $(SRC))
COUNT		:= 0

$(OBJ_DIR)/%.o: %.c
			@mkdir -p $(dir $@)
			@$(CC) -c $(CFLAGS) $(HEADER) $< -o $@
			@$(eval COUNT=$(shell echo $$(($(COUNT)+1))))
			@PERCENT=$$(($(COUNT)*99/$(TOTAL))) ; \
			BAR=$$(($(COUNT)*39/$(TOTAL))) ; \
			if [ $$PERCENT -lt 33 ]; then \
				COLOR_CODE=196; \
			elif [ $$PERCENT -lt 66 ]; then \
				COLOR_CODE=208; \
			else \
				COLOR_CODE=226; \
			fi ; \
			printf "\033[38;5;%sm\r[" $$COLOR_CODE ; \
			i=1; while [ $$i -le $$BAR ]; do printf "▬"; i=$$((i+1)); done ; \
			while [ $$i -le 40 ]; do printf " "; i=$$((i+1)); done ; \
			printf "] LOADING %3d%%\033[0m" $$PERCENT


clean:
			@if ls $(OBJ) >/dev/null 2>&1; then \
			echo "${GREEN}====   $(NAME)   ==== : >>>OBJ CLEANED<<<${RESET}"; \
			fi
			@rm -f $(OBJ)
			@rm -rf $(OBJ_DIR)

fclean:		clean
			@if ls $(NAME) >/dev/null 2>&1; then \
			echo "${GREEN}====   $(NAME)   ==== : >>>ALL CLEANED<<<${RESET}"; \
			fi
			@rm -f $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
