# Distributed under the OSI-approved MIT License.  See accompanying
# file LICENSE or https://github.com/Crascit/DownloadProject for details.

cmake_minimum_required(VERSION 2.8.2)

project(cpr-download NONE)

include(ExternalProject)
ExternalProject_Add(cpr-download
                    GIT_REPOSITORY;https://github.com/whoshuu/cpr.git;GIT_TAG;41fbaca;UPDATE_DISCONNECTED;1
                    SOURCE_DIR          "/home/joshuamuthii/programming/LeetBot/deps/sleepy-discord/deps/cpr"
                    BINARY_DIR          "/home/joshuamuthii/programming/LeetBot/build/cpr-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
)
