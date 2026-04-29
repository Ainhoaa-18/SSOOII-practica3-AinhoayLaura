CC = g++
CFLAGS = -std=c++11 -Wall -pthread -Iinclude

SRC_DIR = src
OBJ_DIR = obj
EXE_DIR = exec

TARGET = buscador

OBJS = \
	$(OBJ_DIR)/Buscador.o \
	$(OBJ_DIR)/SistemaPago.o \
	$(OBJ_DIR)/Usuario.o \
	$(OBJ_DIR)/BusquedaArchivos.o

all: $(EXE_DIR)/$(TARGET)

$(EXE_DIR)/$(TARGET): $(OBJS)
	mkdir -p $(EXE_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o $(EXE_DIR)/$(TARGET) $(TARGET)

run: all
	$(EXE_DIR)/$(TARGET)

.PHONY: all clean run