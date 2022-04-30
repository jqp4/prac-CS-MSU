CC = clang++
CC_FLAGS = -std=c++17 -Werror -g
RUN_FILE_NAME = run

all: cleaning formatting compilation
	./$(RUN_FILE_NAME)
	rm -rf $(RUN_FILE_NAME).dSYM
	rm $(RUN_FILE_NAME)

compilation:
	$(CC) $(CC_FLAGS) *.cpp -o $(RUN_FILE_NAME)

formatting:
	clang-format -i -style=Chromium *.hpp
	clang-format -i -style=Chromium *.cpp

cleaning:
	clear
