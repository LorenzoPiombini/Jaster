TARGET := parser
SRC := $(wildcard src/*.c)
OBJ := $(patsubst src/%.c, obj/%.o, $(SRC))


default: $(TARGET)

clean:
	rm -f obj/*.o
	rm $(TARGET)

$(TARGET): $(OBJ)
	gcc -o $@ $? -fsanitize=address 



obj/%.o: src/%.c
	gcc -Wall -Wextra -g3 -c $< -o $@ -Iinclude -fsanitize=address
