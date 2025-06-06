# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/joshuamuthii/programming/discord_leetcode_bot/deps/sleepy-discord/deps/cpr"
  "/home/joshuamuthii/programming/discord_leetcode_bot/build/cpr-build"
  "/home/joshuamuthii/programming/discord_leetcode_bot/build/cpr-download/cpr-download-prefix"
  "/home/joshuamuthii/programming/discord_leetcode_bot/build/cpr-download/cpr-download-prefix/tmp"
  "/home/joshuamuthii/programming/discord_leetcode_bot/build/cpr-download/cpr-download-prefix/src/cpr-download-stamp"
  "/home/joshuamuthii/programming/discord_leetcode_bot/build/cpr-download/cpr-download-prefix/src"
  "/home/joshuamuthii/programming/discord_leetcode_bot/build/cpr-download/cpr-download-prefix/src/cpr-download-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/joshuamuthii/programming/discord_leetcode_bot/build/cpr-download/cpr-download-prefix/src/cpr-download-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/joshuamuthii/programming/discord_leetcode_bot/build/cpr-download/cpr-download-prefix/src/cpr-download-stamp${cfgdir}") # cfgdir has leading slash
endif()
