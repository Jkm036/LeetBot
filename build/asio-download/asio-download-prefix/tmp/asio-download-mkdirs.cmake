# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/joshuamuthii/programming/LeetBot/deps/sleepy-discord/deps/asio"
  "/home/joshuamuthii/programming/LeetBot/build/asio-build"
  "/home/joshuamuthii/programming/LeetBot/build/asio-download/asio-download-prefix"
  "/home/joshuamuthii/programming/LeetBot/build/asio-download/asio-download-prefix/tmp"
  "/home/joshuamuthii/programming/LeetBot/build/asio-download/asio-download-prefix/src/asio-download-stamp"
  "/home/joshuamuthii/programming/LeetBot/build/asio-download/asio-download-prefix/src"
  "/home/joshuamuthii/programming/LeetBot/build/asio-download/asio-download-prefix/src/asio-download-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/joshuamuthii/programming/LeetBot/build/asio-download/asio-download-prefix/src/asio-download-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/joshuamuthii/programming/LeetBot/build/asio-download/asio-download-prefix/src/asio-download-stamp${cfgdir}") # cfgdir has leading slash
endif()
