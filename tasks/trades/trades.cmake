# trades.cmake

include_directories(index)
include_directories(reader)

# Add source files
set(TRADES_SOURCE_FILES
        reader/reader.cpp
        index/index.cpp
)

# Create object targets
add_library(trades_reader OBJECT reader/reader.cpp)
add_library(trades_index OBJECT index/index.cpp)

# Create static library target
add_library(trades STATIC $<TARGET_OBJECTS:trades_reader> $<TARGET_OBJECTS:trades_index>)