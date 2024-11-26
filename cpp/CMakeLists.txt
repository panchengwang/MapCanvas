cmake_minimum_required(VERSION 3.14)

project(mapcanvas LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add_compile_options( -Wextra -Wpedantic -Wno-unused-result)


find_package(PkgConfig REQUIRED)
pkg_check_modules(JSON_C REQUIRED json-c)
pkg_check_modules(CAIRO REQUIRED cairo)
pkg_check_modules(GEOS REQUIRED geos)

execute_process(COMMAND  pg_config --includedir
    OUTPUT_VARIABLE PG_INC_DIR
)
execute_process(COMMAND  pg_config --libdir
    OUTPUT_VARIABLE PG_LIB_DIR
)
execute_process(COMMAND  pg_config --bindir
    OUTPUT_VARIABLE PG_BIN_DIR
)
string(REGEX REPLACE "\n$" "" PG_INC_DIR "${PG_INC_DIR}")
string(REGEX REPLACE "\n$" "" PG_LIB_DIR "${PG_LIB_DIR}")
string(REGEX REPLACE "\n$" "" PG_BIN_DIR "${PG_BIN_DIR}")


list(APPEND HEADERS 
  Canvas.h
)

list(APPEND SOURCES 
  Canvas.cpp
)

add_library(mapcanvas SHARED
  ${HEADERS}
  ${SOURCES}

)

message(STATUS geos ${GEOS_LIBRARIES})

target_link_libraries(mapcanvas 
  ${JSON_C_LIBRARIES}
  ${CAIRO_LIBRARIES}
  ${GEOS_LIBRARIES}
  mapsymbol
  geos
)
target_compile_definitions(mapcanvas PRIVATE AS_LIBRARY)
target_include_directories(mapcanvas PUBLIC 
  ${JSON_C_INCLUDE_DIRS}
  ${CAIRO_INCLUDE_DIRS}
  ${GEOS_LIBRARIES}
  ${PG_INC_DIR}
)
target_link_directories(mapcanvas PUBLIC
  ${GEOS_LIBRARY_DIRS}
  ${PG_LIB_DIR}
  ${PG_BIN_DIR}
)
target_compile_options(mapcanvas PUBLIC 
  ${JSON_C_CFLAGS_OTHER}
  ${CAIRO_CFLAGS_OTHER}
  )

include(GNUInstallDirs)

install(TARGETS mapcanvas
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_custom_command(TARGET mapcanvas POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:mapcanvas> ${CMAKE_BINARY_DIR}/bin/$<TARGET_FILE_NAME:mapcanvas>
)

install(FILES ${HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mapcanvas
)
