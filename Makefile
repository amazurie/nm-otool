# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amazurie <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/11/05 18:23:34 by lbopp             #+#    #+#              #
#    Updated: 2019/03/14 14:59:28 by amazurie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NM	= ft_nm
OTOOL	= ft_otool

CC	= gcc
FLAGS	= -Wall -Wextra -Werror
DFLAGS	= -MMD
LIB	= libft/libft.a
HEADER	= includes/
HEAD	= ft_nm_otool.h
LIBSRC	= libft/
OBJDIR	= objs/
SRCDIR	= srcs/

SRC	=	nm_otool.c \
		handle_32_64.c \
		handle_fat.c \
		handle_arch.c \
		sect.c \
		syml.c \
		type.c \
		sort.c \
		show.c \
		show_mem.c \
		utils.c

NM_SRC	=	ft_nm.c

OT_SRC	=	ft_otool.c

# colors
GRN     =   \033[0;32m
RED     =   \033[0;31m
CYN     =   \033[0;36m
NC      =   \033[0m

NM_SRCS	= $(addprefix $(SRCDIR), $(NM_SRC))
NM_OBJS	= $(addprefix $(OBJDIR), $(NM_SRC:.c=.o))
NM_DEPS	:= $(NM_OBJS:.o=.d)

OT_SRCS	= $(addprefix $(SRCDIR), $(OT_SRC))
OT_OBJS	= $(addprefix $(OBJDIR), $(OT_SRC:.c=.o))
OT_DEPS	:= $(OT_OBJS:.o=.d)

SRCS	= $(addprefix $(SRCDIR), $(SRC))
OBJS	= $(addprefix $(OBJDIR), $(SRC:.c=.o))
DEPS	:= $(OBJS:.o=.d)

all: checklib $(OBJDIR) $(NM) $(OTOOL)

checklib:
	@make -C $(LIBSRC)

$(NM): $(LIB) $(NM_OBJS) $(OBJS)
	@$(CC) $(FLAGS) -L./$(LIBSRC) -lft -ltermcap -o $(NM) $(NM_OBJS) $(OBJS)
	@echo "\r\c"
	@tput cd
	@echo "${GRN}$(NM) created${NC}"
	@echo "${CYN}PROCESSING DONE !\n${NC}"

$(OTOOL): $(LIB) $(OT_OBJS) $(OBJS)
	@$(CC) $(FLAGS) -L./$(LIBSRC) -lft -ltermcap -o $(OTOOL) $(OT_OBJS) $(OBJS)
	@echo "\r\c"
	@tput cd
	@echo "${GRN}$(OTOOL) created${NC}"
	@echo "${CYN}PROCESSING DONE !\n${NC}"

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(LIB):
	@echo "${CYN}Processing ${NC}./libft ${CYN}[${NC}...${CYN}]${NC}"
	@make checklib
	@echo "\n${CYN}Processing ${NC}./objs ${CYN}[${NC}...${CYN}]${NC}"

$(OBJDIR)%.o: $(SRCDIR)%.c $(HEADER)/$(HEAD)
	@echo "\r\c"
	@tput cd
	@echo "${GRN}Compiling${NC} $@\c"
	@$(CC) $(FLAGS) $(DFLAGS) -c -o $@ $< -I $(HEADER)

clean:
	@echo "${RED}Cleaning ${NC}$(OBJDIR) ${RED}[${NC}...${RED}]${NC}"
	@rm -rf $(OBJS)
	@rm -rf $(DEPS)
	@echo "${RED}Cleaning ${NC}$(LIBSRC) ${RED}[${NC}...${RED}]${NC}"
	@make -C $(LIBSRC) clean

fclean: clean
	@echo "${RED}Cleaning ${NC}./${RED}$(NM)${NC}"
	@rm -rf $(NM)
	@echo "${RED}Cleaning ${NC}./${RED}$(OTOOL)${NC}"
	@rm -rf $(OTOOL)
	@echo "${RED}Cleaning ${NC}$(LIBSRC)${RED}libft.a${NC}\n"
	@make -C $(LIBSRC) fclean
	@rm -rf $(OBJDIR)
	@echo "${RED}DELETE DONE !${NC}\n"

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
