CXXFLAGS := -pedantic-errors -Wall
LDLIBS := -lSDL2 -lm

# Diretórios importantes.
SRC_DIR := src/
INCLUDE_DIR := include/
BUILD_DIR := build/

# Lista de arquivos .cpp
CPP_FILES := $(shell find $(SRC_DIR) -name "*.cpp")

# Lista de arquivos .o
OBJ_FILES := $(patsubst $(SRC_DIR)%.cpp, $(BUILD_DIR)%.o, $(CPP_FILES))

# Lista de headers sem implementação.
HEADERSONLY := $(filter-out $(patsubst $(SRC_DIR)%.cpp, $(INCLUDE_DIR)%.hpp, $(CPP_FILES)), $(shell find $(INCLUDE_DIR) -name "*.hpp"))

ALL: $(BUILD_DIR) main

# Target para gerar executável para debugging.
debug: CXXFLAGS += -g
debug: clean ALL

# Target para ligação.
main: $(BUILD_DIR)main.o $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $@ $(LDLIBS)

# Targets para compilação.
$(BUILD_DIR)main.o: $(SRC_DIR)main.cpp $(HEADERSONLY)
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I $(INCLUDE_DIR)

$(BUILD_DIR)%.o: $(SRC_DIR)%.cpp $(INCLUDE_DIR)%.hpp $(HEADERSONLY)
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I $(INCLUDE_DIR)

# Target para criar o diretório build.
$(BUILD_DIR):
ifeq ($(wildcard $(BUILD_DIR)),)
    $(shell mkdir $(BUILD_DIR))
    $(shell find $(INCLUDE_DIR) -type d -exec mkdir -p "$(BUILD_DIR){}" \;)
    $(shell mv $(BUILD_DIR)$(INCLUDE_DIR)* $(BUILD_DIR) 2>/dev/null ; true) # "2>/dev/null" faz um possível erro do mv ser ignorado
    $(shell rm -r $(BUILD_DIR)$(INCLUDE_DIR))
endif

# Target para apagar objetos e executável.
clean:
	rm -rf $(BUILD_DIR) *.o main
	clear