set(CMAKE_CXX_FLAGS "-std=c++14")

set(SERVER_SOURCES
  main.cpp
  Console.cpp
  Zv8/V8EngineFactory.cpp
  Zv8/V8Engine.cpp
  Zv8/V8Module.cpp
  )

find_package(Boost REQUIRED program_options)
find_package(V8 REQUIRED)
find_package(IndieZen REQUIRED)

include_directories( ${V8_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} )

add_executable(zenserverd ${SERVER_SOURCES})

target_link_libraries(zenserverd  ${ZEN_SCRIPTING_LIBRARY} ${V8_LIBRARIES} ${Boost_LIBRARIES} )
