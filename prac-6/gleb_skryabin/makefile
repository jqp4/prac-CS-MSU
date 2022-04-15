CC = clang++
CC_FLAGS = -std=c++17 -Werror -g
RUN_FILE_NAME = run

all: cleaning formatting compilation
	./$(RUN_FILE_NAME)
	python3 Pods/vis.py
	rm -rf $(RUN_FILE_NAME).dSYM
	rm $(RUN_FILE_NAME)

compilation:
	$(CC) $(CC_FLAGS) *.cpp -o $(RUN_FILE_NAME) -lpthread

formatting:
	clang-format -i -style=Chromium *.hpp
	clang-format -i -style=Chromium *.cpp

cleaning:
	touch ./temp/graph_tmp.json
	rm ./temp/graph_*.json
	clear
