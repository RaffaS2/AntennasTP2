# Compilador
CC = gcc

# Flags
# -Ilib diz ao ficheiro para procurar ficheiros .h dentro da pasta /lib
# -Wall ativa as warnings do compiler 
CFLAGS = -Wall -Ilib
# ar cria a biblioteca estática
AR = ar
# replace, create,symbol table
ARFLAGS = rcs

# Ficheiros
SRC_LIB = functions.c
OBJ_LIB = functions.o
LIB = libfunctions.a
MAIN = main.c
TARGET = graphTP

#cria a lib e o exe
all: $(LIB) $(TARGET)

# Compilar a biblioteca
# Compila functions.c para um ficheiro objeto functions.o.
$(OBJ_LIB): $(SRC_LIB)
	$(CC) $(CFLAGS) -c $(SRC_LIB) -o $(OBJ_LIB)

# ar Cria a biblioteca estática
$(LIB): $(OBJ_LIB)
	$(AR) $(ARFLAGS) $(LIB) $(OBJ_LIB)

# Compilar o executável
$(TARGET): $(MAIN) $(LIB)
	$(CC) $(CFLAGS) $(MAIN) -L. -lfunctions -o $(TARGET)

#gcc -Wall -Ilib -c functions.c -o functions.o
#ar rcs libfunctions.a functions.o

# Limpar
clean:
	rm -f $(OBJ_LIB) $(LIB) $(TARGET)

# Executar
run: all
	./$(TARGET)

