
set(APP_TARGET app)

add_executable(${APP_TARGET}
        ${LIBRARY_SOURCES}
        main.cpp
)

target_include_directories(${APP_TARGET} PRIVATE
        ${LIBRARY_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
        ${CMAKE_BINARY_DIR}
)

target_link_libraries(${APP_TARGET}
        ${Boost_LIBRARIES}
        ${SQLite3_LIBRARIES}
        spdlog::spdlog_header_only
)
