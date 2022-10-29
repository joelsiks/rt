CXX = g++
CXXFLAGS = -Wall -Wextra
LINK_FLAGS = -pthread	
DEBUG_FLAGS = -g

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

OUT = $(BUILD_DIR)/rt
OUT_IMAGE = image.ppm

SRCS = main.cpp thread_pool.cpp
OBJS = $(SRCS:.cpp=.o)
BUILD_SRCS = $(addprefix $(BUILD_DIR)/, $(OBJS))

# -----------------------

always: build

debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: build

prep:
	@mkdir -p $(BUILD_DIR)

build: prep $(OUT)

$(OUT): $(BUILD_SRCS)
	$(CXX) $^ $(CXXFLAGS) $(LINK_FLAGS) -o $(OUT)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -I$(INCLUDE_DIR) -c $(CXXFLAGS) -o $@ $<

run: build
	@./$(OUT)

image: build
	@./$(OUT) > $(OUT_IMAGE)

format:
	astyle --style=attach \
		   --indent=spaces=4 \
		   -n \
		   $(SRC_DIR)/*.cpp $(INCLUDE_DIR)/*.h

clean:
	@rm -rf $(BUILD_DIR) $(OUT_IMAGE)
	@echo "Clean!"

.PHONY: build run