project(PNGDecoder)

find_package(ZLIB REQUIRED)
find_package(Boost REQUIRED)

set(PNG_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(${PROJECT_NAME} ${SOURCES})

target_sources(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/png_decoder.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/inflater.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/crc.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} PUBLIC ZLIB::ZLIB)
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})

set(PNG_STATIC ${PROJECT_NAME})