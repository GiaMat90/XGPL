file(GLOB INCLUDES_LIST "include/*.h" )
file(GLOB SOURCES_LIST "src/*.cpp" )

include_directories(../gpl/include)

add_executable(SlavePrompt main.cpp slave_prompt.cpp slave_prompt.h)

# add link to the lib gpl
target_link_libraries(SlavePrompt PUBLIC gpl)

target_compile_features(SlavePrompt PUBLIC cxx_std_17)
