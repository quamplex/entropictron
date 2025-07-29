set(ENT_STANDALONE_SOURCES ${ENT_STANDALONE_DIR}/main.cpp)

add_executable(entropictron
  ${ENT_COMMON_HEADERS}
  ${ENT_COMMON_RESOURCES}
  ${ENT_STANDALONE_SOURCES})

add_dependencies(entropictron api_standalone)
add_dependencies(entropictron redkite)
target_link_libraries(entropictron entropictron_common api_standalone redkite)
if (CMAKE_SYSTEM_NAME MATCHES Windows)
    target_link_libraries(entropictron "-mwindows -lstdc++ -lstdc++fs -lsndfile -lm -lpthread")
else()
    target_link_libraries(entropictron "-lstdc++fs")
    target_link_libraries(entropictron "-lX11 -ljack -lsndfile -lrt -lm -ldl -lpthread -lcairo")
endif()

install(TARGETS entropictron DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
#install(FILES ${ENT_DOC_DIR}/man/${target}.1  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/man/man1)

