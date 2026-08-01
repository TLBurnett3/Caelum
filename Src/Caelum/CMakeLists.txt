


# Collect application source files
set(APP_SOURCES
    Main.cpp
)

# Define the application executable target
add_executable(Caelum ${APP_SOURCES})

# Link against our custom CaelumLib target
# This automatically inherits include paths and OpenCV linkage from CaelumLib
target_link_libraries(Caelum
    PRIVATE
        CaelumLib
)