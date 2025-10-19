if(CONFIG_ZMK_KEYBOARD_NAME STREQUAL "Keyball44")
    target_sources(app PRIVATE behavior_pmw_timeout.c)
    
    zephyr_library_include_directories(${CMAKE_SOURCE_DIR}/config/boards/shields/keyball_nano)
    list(APPEND DTS_ROOT ${CMAKE_SOURCE_DIR}/config/boards/shields/keyball_nano)
endif()

