# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/domenico/Desktop/PCS2023/PCS2023_Exercises/externals/Main_Source/googletest"
  "/Users/domenico/Desktop/PCS2023/PCS2023_Exercises/externals/Main_Build/googletest"
  "/Users/domenico/Desktop/PCS2023/PCS2023_Exercises/externals/googletest-prefix"
  "/Users/domenico/Desktop/PCS2023/PCS2023_Exercises/externals/googletest-prefix/tmp"
  "/Users/domenico/Desktop/PCS2023/PCS2023_Exercises/externals/googletest-prefix/src/googletest-stamp"
  "/Users/domenico/Desktop/PCS2023/PCS2023_Exercises/externals/googletest-prefix/src"
  "/Users/domenico/Desktop/PCS2023/PCS2023_Exercises/externals/googletest-prefix/src/googletest-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/domenico/Desktop/PCS2023/PCS2023_Exercises/externals/googletest-prefix/src/googletest-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/domenico/Desktop/PCS2023/PCS2023_Exercises/externals/googletest-prefix/src/googletest-stamp${cfgdir}") # cfgdir has leading slash
endif()
