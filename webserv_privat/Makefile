# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jseidere <jseidere@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/27 08:19:55 by miheider          #+#    #+#              #
#    Updated: 2025/03/05 13:13:56 by jseidere         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

###############################################################################
######                            PROPERTIES                             ######
###############################################################################

CXX         = c++
RM          = rm -rf
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98 -pedantic -I./includes -MD -MP -g
MAKEFLAGS   = -j$(nproc) --no-print-directory

NAME        = webserv
SRCSDIR     = srcs
OBJSDIR     = objs
TESTDIR     = tests
TESTNAME    = test_request

# Sources for the main program
SRCS        =	\
				$(SRCSDIR)/Request.cpp \
				$(SRCSDIR)/main.cpp \
				$(SRCSDIR)/Server.cpp \
				$(SRCSDIR)/Configuration.cpp \
				$(SRCSDIR)/Config_utils.cpp \

# Test sources
TEST_SRCS   = \
				$(TESTDIR)/request_test.cpp \
				$(SRCSDIR)/Request.cpp

# Object files
OBJS        = $(addprefix $(OBJSDIR)/, $(notdir $(SRCS:.cpp=.o)))
DEPS        = $(addprefix $(OBJSDIR)/, $(notdir $(SRCS:.cpp=.d)))
TEST_OBJS   = $(addprefix $(OBJSDIR)/, $(notdir $(TEST_SRCS:.cpp=.o)))

TOTAL_SRCS  = $(words $(SRCS))
COMPILED    = 0

# Rule for displaying progress during compilation
define progress_bar
	$(eval COMPILED=$(shell echo $$(($(COMPILED) + 1))))
	$(eval PROGRESS=$(shell echo $$(($(COMPILED) * 100 / $(TOTAL_SRCS)))))
	@echo -ne "\033[1;34mCompiling [$(PROGRESS)%] $<...\033[0m\r"
endef

###############################################################################
#######                               RULES                               ######
###############################################################################

# Default target
all: $(NAME)

# Rule to build the main project executable
$(NAME): $(OBJS)
	@echo linking...
	@$(CXX) -o $@ $^
	@if [ $$? -eq 0 ]; then echo "\033[1;32mready\033[0m"; else echo "\033[1;31mnot ready\033[0m"; fi

# Rule for compiling the main program source files
$(OBJSDIR)/%.o: $(SRCSDIR)/%.cpp
	@echo "Compiling: $< -> $@"
	@mkdir -p $(OBJSDIR)
	@echo loading $<...
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@if [ -f "$@" ]; then echo "\033[1;32m[✔] Compiled: $@\033[0m"; else echo "\033[1;31m[✘] Failed: $@\033[0m"; exit 1; fi


# Clean up object and executable files
clean:
	@$(RM) $(OBJSDIR)
	@echo "Cleaned"

fclean:
	@$(RM) $(OBJSDIR) $(NAME) $(TESTNAME)

re: fclean 
	@echo "Rebuilding project..."
	@$(MAKE) all

# Test target (only compiles the test source files)
test: $(TESTNAME)
	@echo "Running tests..."
	@./$(TESTNAME)

# Rule to compile the test sources (Request.cpp and request_test.cpp)
$(TESTNAME): $(TEST_OBJS)
	@echo "Compiling test..."
	@$(CXX) $(CXXFLAGS) -o $(TESTNAME) $(TEST_OBJS)
	@echo "\033[1;32mTest compiled successfully\033[0m"

# Rule for compiling the test files
$(OBJSDIR)/%.o: $(TESTDIR)/%.cpp
	@mkdir -p $(OBJSDIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Include dependency files
-include $(DEPS)

.PHONY: all clean fclean re test