# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\eco_vision_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\eco_vision_autogen.dir\\ParseCache.txt"
  "eco_vision_autogen"
  )
endif()
