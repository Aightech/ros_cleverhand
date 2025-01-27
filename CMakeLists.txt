cmake_minimum_required(VERSION 3.8)
project(rosCleverHand)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

add_executable(pub_dummy src/pub_dummy.cpp)
ament_target_dependencies(pub_dummy rclcpp std_msgs)

install(TARGETS
  pub_dummy
  DESTINATION lib/${PROJECT_NAME})

ament_package()
