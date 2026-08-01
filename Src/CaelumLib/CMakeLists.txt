# Collect library source files
set(LIB_INCLUDES
    Include/StarDef.h
    Include/HipparcosLoader.h
)

set(LIB_SOURCES
    HipparcosLoader.cpp
)

# Define the library target
add_library(CaelumLib STATIC  ${LIB_INCLUDES} ${LIB_SOURCES})

# Target-based include directories
# PUBLIC: Any executable linking CaelumLib gets access to CaelumLib/include
target_include_directories(CaelumLib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:Include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link OpenCV dependency into CaelumLib
target_link_libraries(CaelumLib
    PUBLIC
        ${OpenCV_LIBS}
)