# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: capapes <capapes@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/31 09:52:48 by tatahere          #+#    #+#              #
#    Updated: 2026/08/03 12:05:04 by taha             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ROOT_DIR	=	./

BIN_DIR		=	bin/
SRC_DIR		=	src/
INC_DIR		=	include/
TEST_DIR	=	tests/
TEST_BIN_DIR	=	$(BIN_DIR)tests/

RM		=	rm -rf
CXX		=	g++

VPATH	= $(ROOT_DIR):
VPATH	+= $(SRC_DIR):
VPATH	+= $(SRC_DIR)common/containers:
VPATH	+= $(SRC_DIR)common/containers/tree:
VPATH	+= $(SRC_DIR)config:
VPATH	+= $(SRC_DIR)config/lexer:
VPATH	+= $(SRC_DIR)config/parser:
VPATH	+= $(SRC_DIR)config/parser/rules:
VPATH	+= $(SRC_DIR)config/symbols:
VPATH	+= $(SRC_DIR)config/symbols/non_terminal:
VPATH	+= $(SRC_DIR)config/symbols/terminal:
VPATH	+= $(SRC_DIR)config/symbols/terminal/character_lexeme:
VPATH	+= $(SRC_DIR)config/symbols/terminal/keywords:
VPATH	+= $(SRC_DIR)config/symbols/terminal/literals:
VPATH	+= $(SRC_DIR)common/errors:
VPATH	+= $(SRC_DIR)http/uri:
VPATH	+= $(SRC_DIR)http/uri/utils:
VPATH	+= $(SRC_DIR)network:
VPATH	+= $(SRC_DIR)http/request:
VPATH	+= $(SRC_DIR)http/response:
VPATH	+= $(SRC_DIR)server

CXXFLAGS	:=	-Wall -Wextra -Werror -std=c++98 -MMD -g

CINC		:=	-I$(BIN_DIR)
CINC		+=	-I$(INC_DIR)
CINC		+=	-I$(subst : , -I,$(VPATH))
CXXFLAGS	+=	$(CINC)

CDEBUG		:=	-g -fsanitize=address 
CXXFLAGS	+=	$(CDEBUG)

CXXFLAGS	+= $(CXXMACROS)

NAME	=	webserv

OBJ		:=	main.o						\
										\
			args.o						\
			ir_printing_functions.o		\
			configuration_parser.o		\
										\
			parse_exception.o			\
			multy_parse_exception.o		\
										\
			a_symbol.o					\
			a_terminal.o					\
			a_non_terminal.o				\
			a_evaluable.o				\
										\
			a_lexer.o					\
			config_file_lexer.o			\
										\
			a_terminal_factory.o			\
			config_file_terminal_factory.o	\
										\
			number.o					\
			text_config_file.o			\
			white_space.o				\
										\
			a_key_word.o					\
			key_word_server.o				\
			key_word_server_name.o			\
			key_word_listen.o				\
			key_word_client_max_body_size.o	\
			key_word_error_page.o			\
			key_word_location.o			\
			key_word_return.o				\
			key_word_limit_exept.o			\
			key_word_root.o				\
			key_word_auto_index.o			\
			key_word_index.o				\
			key_semicolon.o				\
			key_left_curly_bracket.o		\
			key_right_curly_bracket.o		\
										\
			a_parser.o					\
			a_parsing_rule.o				\
			parsing_rule_symbol.o			\
			parsing_rule_and.o			\
			parsing_rule_or.o				\
			parsing_rule_repetition.o		\
										\
			symbol_config_context.o		\
			symbol_server.o				\
			symbol_server_name.o			\
			symbol_listen.o				\
			symbol_client_max_body_size.o	\
			symbol_error_page.o			\
			symbol_location.o			\
			symbol_limit_except.o			\
			symbol_return.o				\
			symbol_root.o				\
			symbol_auto_index.o			\
			symbol_index.o				\
										\
			tokenize.o					\
			percent_encoding_utils.o	\
			utils.o						\
										\
			scheme.o					\
			user_info.o					\
			ip_literal.o					\
			host.o						\
			authority.o					\
			path.o						\
			query.o						\
			fragment.o					\
			uri.o						\
										\
			epoll_connection_manager.o	\
			socket.o					\
			set_up_sockets.o				\
										\
			field_validators.o			\
			headers.o					\
			req_scanner.o				\
			request.o					\
			schemas.o					\
										\
			server.o					\
			server_manager.o				\
			server_response.o			\


TESTS_SRCS	:=	$(shell find $(TEST_DIR)unit -name "*.test.cpp" | sort)

TESTS_OBJ	:=	$(patsubst $(TEST_DIR)unit/%.cpp, $(TEST_BIN_DIR)%.o,$(TESTS_SRCS))

TESTS		:=	$(patsubst $(TEST_DIR)unit/%.cpp, $(TEST_BIN_DIR)%.run,$(TESTS_SRCS))

OBJ		:=	$(addprefix $(BIN_DIR), $(OBJ))

DEP		:=	$(OBJ:.o=.d) $(TESTS_OBJ:.o=.d)

all	:	$(BIN_DIR) $(NAME)

$(BIN_DIR)		:
	mkdir -p $(BIN_DIR)

$(NAME) : $(OBJ) Makefile
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

$(BIN_DIR)%.o : %.cpp	Makefile | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_BIN_DIR)%.o : $(TEST_DIR)unit/%.cpp	Makefile
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_BIN_DIR)%.run: $(TEST_BIN_DIR)%.o $(filter-out $(BIN_DIR)main.o,$(OBJ)) Makefile
	$(CXX) $(CXXFLAGS) $< $(filter-out $(BIN_DIR)main.o,$(OBJ)) -o $@

clean :
	$(RM) $(BIN_DIR)

fclean : clean
	$(RM) $(NAME)

re : fclean all

demo: all
	rm -rf /tmp/http_server
	cp -r ./examples/complete_example/http_server /tmp/http_server/
	./webserv ./config/demo.conf
	

test : all build_test
	@echo //
	@echo //	runing all the tests
	@echo //
	zsh run_tests.sh $(TEST_BIN_DIR)

build_test: $(TESTS)

run: demo


-include $(DEP)

.SECONDARY: $(TESTS_OBJ)

.PHONY:	all clean fclean re demo test build_test run
