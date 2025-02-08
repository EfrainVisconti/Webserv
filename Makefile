USER_NAME      = eviscont, jde-orma & adlopez

# Colors:
DEF_COLOR	   = \033[0;39m
BLUE		   = \033[0;94m
PURPLE         = \033[0;95m
GREEN		   = \033[0;92m

# Compiler and flags
CC             = c++
CFLAGS         = -Wall -Wextra -Werror -std=c++98 -g3

RM             = rm -rf

# Directories
SRC_DIR        = src/
INC_DIR        = inc/
OBJ_DIR        = obj/
BIN_DIR        = ./

# Source files
SRC_FILE       = main.cpp Server.cpp ServerManager.cpp CgiManager.cpp Request.cpp Response.cpp
INC_FILE       = main.hpp Server.hpp ServerManager.hpp CgiManager.hpp Request.hpp Response.hpp

SRC            = $(addprefix $(SRC_DIR), $(SRC_FILE))
INC            = $(addprefix $(INC_DIR), $(INC_FILE))
OBJ_FILE       = $(SRC_FILE:.cpp=.o)
OBJ            = $(addprefix $(OBJ_DIR), $(OBJ_FILE))

# Binary name for your program
CODE_BIN       = $(BIN_DIR)webserv

# Output executable
NAME           = webserv

all: $(OBJ_DIR) $(CODE_BIN)

$(CODE_BIN): $(OBJ)
	@echo "Compiling $(CODE_BIN)..."
	@$(CC) $(CFLAGS) $(OBJ) -o $(CODE_BIN)
	@echo "$(GREEN)✔ $(BLUE)$(USER_NAME)'s $(PURPLE)$(NAME)$(BLUE) compilation$(DEF_COLOR)"

# Crear obj/ antes de compilar objetos
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Compilar archivos fuente en objetos
$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp | $(OBJ_DIR)
	@echo "Compiling $< -> $@"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJ_DIR)
	@echo "$(GREEN)✔ $(BLUE)$(USER_NAME)'s $(PURPLE)$(NAME)$(BLUE) .o files removal$(DEF_COLOR)"

fclean: clean
	@$(RM) $(CODE_BIN)
	@echo "$(GREEN)✔ $(BLUE)$(USER_NAME)'s $(PURPLE)$(NAME)$(BLUE) executable file removal$(DEF_COLOR)"

re: fclean all

.PHONY: all clean fclean re
