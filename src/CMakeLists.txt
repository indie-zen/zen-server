set(SERVER_SOURCES
  main.cpp
  )

find_package(V8 REQUIRED)

include_directories( ${V8_INCLUDE_DIR} )

add_executable(zenserverd ${SERVER_SOURCES})

target_link_libraries(zenserverd  ${V8_LIBRARIES})
