# Gomoku
NAME			=	Gomoku

OBJS			=	build \
					dist

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

clean:
			@$(RM) $(OBJS) $(NAME).spec
			@printf "\n$(BLUE)==> $(RED)Removed Gomoku 🗑️\n$(RESET)"

fclean:		clean
			@$(RM) $(NAME)

re:			fclean all
			@printf "$(BLUE)==> $(CYAN)Gomoku recompiled 🔄\n$(RESET)"

.PHONY:		all clean fclean re
