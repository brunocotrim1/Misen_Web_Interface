cmake_minimum_required(VERSION 3.10)
find_package(Threads REQUIRED)
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CLIENT_DIR ..)
set(LIB_UNTRUSTED ../../LibUntrusted)
set(FRAMEWORK_include ../../../include)

include_directories("${PROJECT_INCLUDE_DIR}")
include_directories("${CLIENT_DIR}")
include_directories("${FRAMEWORK_include}")
include_directories("../rbisen")
include_directories("../../LibUntrusted")


#add_executable(Server src/server.cpp)
add_executable(Server ${CLIENT_DIR}/bisen_tests.cpp ${LIB_UNTRUSTED}/untrusted_util.cpp ${CLIENT_DIR}/util.cpp ${CLIENT_DIR}/rbisen/ClientCrypt.c ${CLIENT_DIR}/rbisen/Utils.c ${CLIENT_DIR}/rbisen/SseClient.cpp ${CLIENT_DIR}/rbisen/EnglishAnalyzer.cpp ${CLIENT_DIR}/rbisen/PorterStemmer.c ${CLIENT_DIR}/rbisen/QueryParser.cpp)


#${CLIENT_DIR}/ImageSearch.cpp ${CLIENT_DIR}/misen_tests.cpp  ${CLIENT_DIR}/visen_tests.cpp

#target_link_libraries(Server LIB_UNTRUSTED)
# target_include_directories(Server PRIVATE  ../../../include ../rbisen)
# # target_link_libraries(Client ${LIB_UNTRUSTED} ${LIB_UNTRUSTED_TLS} ${OpenCV_LIBS} ${OPENSSL_CRYPTO_LIBRARY} config)
###target_link_libraries(Client ${LIB_UNTRUSTED} ${LIB_UNTRUSTED_TLS})
target_link_libraries(Server PRIVATE Threads::Threads)