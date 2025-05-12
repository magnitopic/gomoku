# Gomoku
NAME			=	Gomoku

SRC				=	src/gomoku.cpp \
					src/classes/Player.cpp \
					src/classes/Screen.cpp \
					src/game_config.cpp

OBJS			= $(SRC:.cpp=.o)

# Compiler
CXX				=	c++
RM				=	rm -f
CXXFLAGS		=	-Wall -Werror -Wextra -std=c++98 #-g3 -fsanitize=address
LIBX_FLAGS		=	-framework Cocoa -framework OpenGL -framework IOKit -L$(MLX_SRC)/build -lmlx42 -lglfw

# MLX42
MLX				=	$(MLX_SRC)/build/libmlx42.a
MLX_SRC			=	./MLX42

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
all:		$(MLX) $(NAME)
			@printf "$(BLUE)==> $(CYAN)$(NAME) compiled ✅\n\n$(RESET)"

$(NAME):	$(OBJS)
			@$(CXX) $(CXXFLAGS) $(LIBX_FLAGS) $(OBJS) $(MXL_SRC)$(MLX)  -o $(NAME)

$(MLX):
			@printf "$(BLUE)==> $(CYAN)Building MLX42 library... 🔧$(RESET)\n"
			@cmake -B $(MLX_SRC)/build -S $(MLX_SRC)
			@cmake --build $(MLX_SRC)/build -j4

clean:
			@$(RM) $(OBJS)
			@printf "\n$(BLUE)==> $(RED)Removed $(NAME) 🗑️\n$(RESET)"

fclean:		clean
			@$(RM) $(NAME)

re:			fclean all
			@printf "$(BLUE)==> $(CYAN)$(NAME) recompiled 🔄\n$(RESET)"

.PHONY:		all clean fclean re
