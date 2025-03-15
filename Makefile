TARGET := parser
SRC := $(wildcard src/*.c)
OBJ := $(patsubst src/%.c, obj/%.o, $(SRC))
OBJ_PROD = $(patsubst src/%.c, obj/%_prod.o, $(SRC))

OBJlibjasterPR = obj/jaster_prod.o

LIBNAMEjaster = jaster
LIBDIR = /usr/lib
INCLUDEDIR = /usr/include
SHAREDLIBjaster = $(LIBNAMEjaster).so

default: $(TARGET)

libraryPR: prod
	sudo gcc -Wall -fPIC -shared -o $(SHAREDLIBjaster) $(OBJlibjasterPR)

prod: $(OBJ_PROD)


clean:
	rm -f obj/*.o
	rm $(TARGET)

$(TARGET): $(OBJ)
	gcc -o $@ $? -fsanitize=address 

obj/%.o: src/%.c
	gcc -Wall -Wextra -g3 -c $< -o $@ -Iinclude -fsanitize=address
	

obj/%_prod.o:src/%.c
	gcc -Wall -c $< -o $@ -Iinclude -fstack-protector-strong -D_FORTiFY_SOURCE=2 -fPIC 

install:
	install -d $(INCLUDEDIR)
	install -m 644 include/jaster.h $(INCLUDEDIR)
	install -m 755 $(SHAREDLIBjaster) $(LIBDIR)
	ldconfig


build: libraryPR install
