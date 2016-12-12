set(CMAKE_CXX_FLAGS "-std=c++14")

set(SERVER_SOURCES
  main.cpp
  )

find_package(Boost REQUIRED program_options)
find_package(V8 REQUIRED)

include_directories( ${V8_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} )

add_executable(zenserverd ${SERVER_SOURCES})

target_link_libraries(zenserverd  ${V8_LIBRARIES} ${Boost_LIBRARIES} )
