file(GLOB INCLUDES_LIST "include/*.h" )
file(GLOB SOURCES_LIST "src/*.cpp" )

include_directories(../gpl/include)

add_executable(EvaMan main.cpp evaluation_board_manager.cpp evaluation_board_manager.h)

# add link to the lib gpl
target_link_libraries(EvaMan PUBLIC gpl)

target_compile_features(EvaMan PUBLIC cxx_std_17)
