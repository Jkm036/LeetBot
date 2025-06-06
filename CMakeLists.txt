cmake_minimum_required(VERSION 3.6)
project(leetbot)

# Create an exectuable and list dependencies of
# exectuable
add_executable(leetbot 
    main.cpp
    LeetBotClient.cpp
)
target_include_directories(leetbot PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# What does add_subdirectory() do ?
# 1. Tells CMake to process another CMakeLists.txt
#    file in a subdirectory
# 2. Builds the dependency as part of your project
# 3. Includes source code directlyl into your build
add_subdirectory(deps/sleepy-discord)

# Tool that helps find installed libraries
find_package(PkgConfig)

# Check if we found PkgConfig
if(PkgConfig_FOUND)
    #Check for libcurl and jsoncpp on this device
    pkg_check_modules(CURL libcurl)
    pkg_check_modules(JSONCPP jsoncpp)

    if(CURL_FOUND AND JSONCPP_FOUND)
        # Link our program to both sleepy-discord AND curl
        target_link_libraries(leetbot sleepy-discord ${CURL_LIBRARIES} ${JSONCPP_LIBRARIES})

        # Tell the compiler where to find curls's header files 
        target_include_directories(leetbot PRIVATE ${CURL_INCLUDE_DIRS} ${JSONCPP_INCLUDE_DIRS})

        # Add flags that tells our codea that we have curl & JSONCPP available
        target_compile_definitions(leetbot PRIVATE HAVE_CURL HAVE_JSONCPP)
        message(STATUS "Found libcurl and jsoncpp - LeetCode functionality enabled")
    elseif(CURL_FOUND)
        message(WARNING "jsoncpp not found, leetcode functionality may not work...")
        target_link_libraries(leetbot sleepy-discord ${CURL_LIBRARIES})
        target_include_directories(leetbot PRIVATE ${CURL_INCLUDE_DIRS})
        target_compile_definitions(leetbot PRIVATE HAVE_CURL)
    else()
        message(WARNING "libcurl not found . HTTP requests may not work")
        target_link_libraries(leetbot sleepy-discord)
    endif()
else()
    # If PkgConfig is not found, hail mary attempt at just linking with
    # curl and jsoncpp - may work on some systems
    target_link_libraries(leetbot sleepy-discord curl jsoncpp)
endif()

# Use C++17
set_property(TARGET leetbot PROPERTY CXX_STANDARD 17)

# Make tokens directory if it doesn't exist
add_custom_target(create_token_dir ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/tokens
    )
