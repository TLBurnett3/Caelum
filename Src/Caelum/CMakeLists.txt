


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
        ${OpenCV_LIBS}
)

if(WIN32)
    add_custom_command(TARGET Caelum POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:Caelum>
        $<TARGET_FILE_DIR:Caelum>
        COMMAND_EXPAND_LISTS
    )
endif()