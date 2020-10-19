#############################################################################
## Set minimum required version of cmake, project name and compile options ##
#############################################################################
cmake_minimum_required(VERSION 2.8.3)
project(agribot_tools_management)

#############################################################################
## Find catkin packages and libraries for catkin and system dependencies   ##
#############################################################################
find_package(catkin REQUIRED COMPONENTS
  roscpp
  agribot_msgs
)

################################################
## Declare ROS messages, services and actions ##
################################################

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

###################################
## catkin specific configuration ##
###################################
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES can
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
)

#############################################################################
## Build                                                                   ##
#############################################################################
include_directories(
 include
 ${catkin_INCLUDE_DIRS}
)

link_directories("${PROJECT_SOURCE_DIR}")
add_executable(agribot_tools_management_node src/ToolManager.cpp src/agribot_tools_management_node.cpp)
target_link_libraries(agribot_tools_management_node ${catkin_LIBRARIES})
add_dependencies(agribot_tools_management_node ${catkin_EXPORTED_TARGETS})

#############
## Install ##
#############

install(TARGETS
  agribot_tools_management_node
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)

install(DIRECTORY launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
  PATTERN ".svn" EXCLUDE
)

#############
## Testing ##
#############
