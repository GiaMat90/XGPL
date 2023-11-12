file(GLOB INCLUDES_LIST "include/*.h" )
file(GLOB SOURCES_LIST "src/*.cpp" )

include_directories(../gpl/include)

add_executable(test_app main.cpp test_app.cpp)

# add link to the lib gpl
target_link_libraries(test_app PUBLIC gpl)

target_compile_features(test_app PUBLIC cxx_std_17)
