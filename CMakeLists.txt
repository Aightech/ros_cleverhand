cmake_minimum_required(VERSION 3.8...3.18)
project(ros_cleverhand)



if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)



###### include helper functions ######
include(CMakeFunctions.txt)


if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/lib")
    execute_process(COMMAND git submodule status
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      OUTPUT_VARIABLE SUBMODULES_LIST)
    subdirlist(LIBS "${CMAKE_CURRENT_SOURCE_DIR}/lib/")
    foreach(subdir ${LIBS})
      #check if the library is a unused library
      set(IS_UNUSED 0)
      foreach(lib ${UNUSED_LIBS})
        string(FIND "${lib}" "${subdir}" FOUND)
        if(${FOUND} GREATER -1)
          set(IS_UNUSED 1)
        endif()
      endforeach()
      if(IS_UNUSED EQUAL 1)
        message("is an unused library")
        continue()
      endif()

      string(FIND "${SUBMODULES_LIST}" "${subdir}" FOUND)
      if(${FOUND} GREATER -1 AND NO_CONNECTION EQUAL 1)
        #git submodules init
        execute_process(COMMAND git submodule update --remote --merge --init -- "lib/${subdir}"
          WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_VARIABLE OUTPUT)
      endif()
      subproject_version("lib/${subdir}" subdir_version) #get the version
      message("Adding library ${subdir} version: ${subdir_version}")
      add_subdirectory("lib/${subdir}")

      get_directory_property(libname
        DIRECTORY "lib/${subdir}"
        DEFINITION LIB_NAME)
      list(APPEND EXTRA_LIBS "${libname}")
      link_directories(${CMAKE_SOURCE_DIR}/lib/${subdir}/bin)
    endforeach()
  endif()

# add_subdirectory(lib/cleverhand-interface)
# get_directory_property(CLEVERHAND_INTERFACE_LIB
#   DIRECTORY lib/cleverhand-interface
#   DEFINITION LIB_NAME
# )



add_executable(pub_dummy src/pub_dummy.cpp)
ament_target_dependencies(pub_dummy rclcpp std_msgs)

target_link_libraries(pub_dummy PRIVATE ${CLEVERHAND_INTERFACE_LIB})



install(TARGETS
  pub_dummy
  DESTINATION lib/${PROJECT_NAME})

ament_package()
