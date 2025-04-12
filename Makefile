# Gomoku
NAME			=	Gomoku

OBJS			=	build \
					dist

GOMOKU_SRC		=	src/min-max/min-max.c

C_OBJS			=	src/min-max/min-max.so

CC				=	cc
CCFLAGS			=	-shared -Wall -Werror #-Wextra


RM				=	rm -fr

# Colours
RED				=	\033[0;31m
GREEN			=	\033[0;32m
YELLOW			=	\033[0;33m
BLUE			=	\033[0;34m
PURPLE			=	\033[0;35m
CYAN			=	\033[0;36m
WHITE			=	\033[0;37m
RESET			=	\033[0m

# Rules
all:		$(NAME)
			@printf "$(BLUE)==> $(CYAN)Gomoku compiled ✅\n\n$(RESET)"

$(NAME):	
			@pyinstaller --onefile --distpath . src/gomoku.py
			@rm -f $(NAME).spec
			@${CC} $(CCFLAGS) -o $(C_OBJS) $(GOMOKU_SRC)

clean:
			@$(RM) $(OBJS) $(NAME).spec
			@$(RM) $(C_OBJS)
			@printf "\n$(BLUE)==> $(RED)Removed Gomoku 🗑️\n$(RESET)"

fclean:		clean
			@$(RM) $(NAME)

re:			fclean all
			@printf "$(BLUE)==> $(CYAN)Gomoku recompiled 🔄\n$(RESET)"

.PHONY:		all clean fclean re
