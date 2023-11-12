file(GLOB INCLUDES_LIST "include/*.h" )
file(GLOB SOURCES_LIST "src/*.cpp" )

include_directories(include)

add_library(gpl SHARED ${SOURCES_LIST} ${INCLUDES_LIST})

add_compile_definitions(GPL_EXPORT)

target_compile_features(gpl PUBLIC cxx_std_17)
