
find_path(V8_INCLUDE_DIR 
    v8.h ${VENDOR_DIR}/v8/include
)

find_library(V8_LIBRARY
    NAMES
      libv8.so
    HINTS
       ${VENDOR_DIR}/v8/out.gn/x64.release
)

find_library(V8_ICU_I18N
    NAMES
        libicui18n.so
    HINTS
       ${VENDOR_DIR}/v8/out.gn/x64.release
)

find_library(V8_ICU_UC
    NAMES
        libicuuc.so
    HINTS
       ${VENDOR_DIR}/v8/out.gn/x64.release
)

set(V8_ICU_LIBS ${V8_ICU_I18N} ${V8_ICU_UC})
set(V8_LIBRARIES ${V8_ICU_LIBS} ${V8_LIBRARY} )

mark_as_advanced(V8_INCLUDE_DIR)
mark_as_advanced(V8_LIBRARIES)


if(V8_INCLUDE_DIR AND V8_LIBRARIES)
  set(V8_FOUND TRUE)
endif(V8_INCLUDE_DIR AND V8_LIBRARIES)

if(V8_FOUND)
  if(NOT V8_FIND_QUIETLY)
    message(STATUS "Found V8: ${V8_LIBRARIES}")
  endif(NOT V8_FIND_QUIETLY)
else(V8_FOUND)
  if(V8_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find V8 library.")
  endif(V8_FIND_REQUIRED)
endif(V8_FOUND)

