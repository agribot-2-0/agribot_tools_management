#############################################################################
## Set minimum required version of cmake, project name and compile options ##
#############################################################################
cmake_minimum_required(VERSION 2.8.3)
project(agribot_tools_management)

add_compile_options(-std=c++17)

#############################################################################
## Find catkin packages and libraries for catkin and system dependencies   ##
#############################################################################
find_package(catkin REQUIRED COMPONENTS
  roscpp
  hardware_interface
  controller_manager
  joint_limits_interface
  control_toolbox
  control_msgs
  agribot_msgs
  agribot_common
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
add_executable(agribot_tools_management_node src/node/ToolManagerNode.cpp src/scanner/Scanner.cpp src/tpl_control/control_arbiter/TplControlBroadcaster.cpp src/tpl_control/control_arbiter/TplArbiter.cpp src/node/tool_manager_node.cpp )
target_link_libraries(agribot_tools_management_node ${catkin_LIBRARIES})
add_dependencies(agribot_tools_management_node ${catkin_EXPORTED_TARGETS})

link_directories("${PROJECT_SOURCE_DIR}")
add_executable(tool_control_node src/node/tool_control_node.cpp src/tpl_control/control/AgribotHardwareInterface.cpp)
target_link_libraries(tool_control_node ${catkin_LIBRARIES})
add_dependencies(tool_control_node ${catkin_EXPORTED_TARGETS})

#############
## Install ##
#############

install(TARGETS
  agribot_tools_management_node
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)

install(TARGETS
  tool_control_node
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)

install(DIRECTORY launch param
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
  PATTERN ".svn" EXCLUDE
)

#############
## Testing ##
#############
