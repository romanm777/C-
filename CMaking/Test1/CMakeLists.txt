CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
include(proj/project.cmake)

# if you are building in-source, this is the same as CMAKE_SOURCE_DIR, otherwise 
# this is the top level directory of your build tree 
MESSAGE( STATUS "CMAKE_BINARY_DIR:         " ${CMAKE_BINARY_DIR} )

# if you are building in-source, this is the same as CMAKE_CURRENT_SOURCE_DIR, otherwise this 
# is the directory where the compiled or generated files from the current CMakeLists.txt will go to 
MESSAGE( STATUS "CMAKE_CURRENT_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR} )

# this is the directory, from which cmake was started, i.e. the top level source directory 
MESSAGE( STATUS "CMAKE_SOURCE_DIR:         " ${CMAKE_SOURCE_DIR} )

# this is the directory where the currently processed CMakeLists.txt is located in 
MESSAGE( STATUS "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR} )

# contains the full path to the top level directory of your build tree 
MESSAGE( STATUS "PROJECT_BINARY_DIR: " ${PROJECT_BINARY_DIR} )

# contains the full path to the root of your project source directory,
# i.e. to the nearest directory where CMakeLists.txt contains the PROJECT() command 
MESSAGE( STATUS "PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR} )

# SET(VERSION_MAJOR "1")
# SET(VERSION_MINOR "0")
# SET(VERSION_PATCH "0")
# SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_MAJOR}")

# #set env
# # ------------------
# add_definition(-DDEF_RET_YES)
# include_directories(${PROJECT_SOURCE_DIR}/inc)
# link_directories(${PROJECT_SOURCE_DIR}/external/lib)

# message ("Binary Tree path: ${PROJECT_BINARY_DIR}")

# #add subfolders
# # -------
# add_subdirectory(libssrc)
# add_subdirectory(dlls)
# add_subdirectory(exes)

# MESSAGE( STATUS "CMAKE_BINARY_DIR (${CMAKE_SOURCE_DIR}/bin): " ${CMAKE_BINARY_DIR} )
# MESSAGE( STATUS "EXECUTABLE_OUTPUT_PATH (${CMAKE_BINARY_DIR}): " ${EXECUTABLE_OUTPUT_PATH} )
# MESSAGE( STATUS "LIBRARY_OUTPUT_PATH (${CMAKE_BINARY_DIR}): " ${LIBRARY_OUTPUT_PATH} )
