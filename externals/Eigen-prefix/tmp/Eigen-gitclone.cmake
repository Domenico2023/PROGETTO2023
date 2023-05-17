# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

if(EXISTS "/Users/domenico/Desktop/PCS2023/PCS2023_Exercises/externals/Eigen-prefix/src/Eigen-stamp/Eigen-gitclone-lastrun.txt" AND EXISTS "/Users/domenico/Desktop/PCS2023/PCS2023_Exercises/externals/Eigen-prefix/src/Eigen-stamp/Eigen-gitinfo.txt" AND
  "/Users/domenico/Desktop/PCS2023/PCS2023_Exercises/externals/Eigen-prefix/src/Eigen-stamp/Eigen-gitclone-lastrun.txt" IS_NEWER_THAN "/Users/domenico/Desktop/PCS2023/PCS2023_Exercises/externals/Eigen-prefix/src/Eigen-stamp/Eigen-gitinfo.txt")
  message(STATUS
    "Avoiding repeated git clone, stamp file is up to date: "
    "'/Users/domenico/Desktop/PCS2023/PCS2023_Exercises/externals/Eigen-prefix/src/Eigen-stamp/Eigen-gitclone-lastrun.txt'"
  )
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/Users/domenico/Desktop/PCS2023/PCS2023_Exercises/externals/Main_Source/eigen3"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/Users/domenico/Desktop/PCS2023/PCS2023_Exercises/externals/Main_Source/eigen3'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git" 
            clone --no-checkout --depth 1 --no-single-branch --progress --config "advice.detachedHead=false" "https://gitlab.com/libeigen/eigen.git" "eigen3"
    WORKING_DIRECTORY "/Users/domenico/Desktop/PCS2023/PCS2023_Exercises/externals/Main_Source"
    RESULT_VARIABLE error_code
  )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once: ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://gitlab.com/libeigen/eigen.git'")
endif()

execute_process(
  COMMAND "/usr/bin/git" 
          checkout "3147391d946bb4b6c68edd901f2add6ac1f31f8c" --
  WORKING_DIRECTORY "/Users/domenico/Desktop/PCS2023/PCS2023_Exercises/externals/Main_Source/eigen3"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: '3147391d946bb4b6c68edd901f2add6ac1f31f8c'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git" 
            submodule update --recursive --init 
    WORKING_DIRECTORY "/Users/domenico/Desktop/PCS2023/PCS2023_Exercises/externals/Main_Source/eigen3"
    RESULT_VARIABLE error_code
  )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/Users/domenico/Desktop/PCS2023/PCS2023_Exercises/externals/Main_Source/eigen3'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy "/Users/domenico/Desktop/PCS2023/PCS2023_Exercises/externals/Eigen-prefix/src/Eigen-stamp/Eigen-gitinfo.txt" "/Users/domenico/Desktop/PCS2023/PCS2023_Exercises/externals/Eigen-prefix/src/Eigen-stamp/Eigen-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/Users/domenico/Desktop/PCS2023/PCS2023_Exercises/externals/Eigen-prefix/src/Eigen-stamp/Eigen-gitclone-lastrun.txt'")
endif()
