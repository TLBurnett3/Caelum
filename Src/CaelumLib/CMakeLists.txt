# Collect library source files
set(LIB_INCLUDES
    Include/Template.h    
    Include/Star.h
    Include/Parser.h
    Include/Catalog.h
    Include/Hipparcos.h
    Include/Tycho-2.h
    Include/Scope.h
    Include/Tracker.h
    Include/OTA.h
    Include/Sensor.h
    Include/ColorLUT.h
)

set(LIB_SOURCES
    Template.cpp
    Parser.cpp
    Hipparcos.cpp
    Tycho-2.cpp
    Scope.cpp
    Catalog.cpp
    Tracker.cpp
    OTA.cpp
    Sensor.cpp
    ColorLUT.cpp
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