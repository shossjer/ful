function(run_tests name executable)
	# inspired by the works of stack overflow user 1178599
	#
	# https://stackoverflow.com/a/53673873
	set(_test_state "${PROJECT_BINARY_DIR}/run_tests/${executable}.passed")
	add_custom_command(
		OUTPUT ${_test_state}
		COMMAND $<TARGET_FILE:${executable}>
		COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/run_tests"
		COMMAND ${CMAKE_COMMAND} -E touch ${_test_state}
		DEPENDS ${executable}
		WORKING_DIRECTORY "${CMAKE_HOME_DIRECTORY}"
		)
	add_custom_target(
		${name}
		ALL
		DEPENDS ${_test_state}
		)
endfunction()

set(_compile_options
	$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
		-Wall -Wextra -Wconversion -Werror -Wno-sign-conversion>
	$<$<AND:$<CXX_COMPILER_ID:Clang>,$<VERSION_LESS:CXX_COMPILER_VERSION,6.0>>:
		-Wno-missing-braces>
	$<$<CXX_COMPILER_ID:MSVC>:
		/W4 /WX
		/Zc:rvalueCast>
	)

set(_compile_definitions
	$<$<CXX_COMPILER_ID:MSVC>:
		NOMINMAX UNICODE _UNICODE>
	)

include (bnc/CMakeLists.txt)
include (cmn/CMakeLists.txt)
include (src/CMakeLists.txt)
include (tst/CMakeLists.txt)

add_library(fullib "")
target_sources(fullib PRIVATE ${FULLIB_SOURCES} ${FULLIB_HEADERS} ${FULLIB_${CMAKE_CXX_COMPILER_ID}})
target_include_directories(fullib PUBLIC "src")
target_link_libraries(fullib PUBLIC lib_cpuid)
target_compile_options(fullib PRIVATE ${_compile_options})
target_compile_definitions(fullib PRIVATE ${_compile_definitions})

if(FUL_BUILD_TESTS OR FUL_BUILD_BENCMARKS)
	add_library(fulcmn "")
	target_sources(fulcmn PRIVATE ${FULCMN_SOURCES} ${FULCMN_HEADERS})
	target_include_directories(fulcmn PUBLIC "cmn")
	target_link_libraries(fulcmn PUBLIC fullib)
	target_compile_options(fulcmn PRIVATE ${_compile_options})
	target_compile_definitions(fulcmn PRIVATE ${_compile_definitions})
endif()

if(FUL_BUILD_TESTS)
	add_executable(fultst "")
	target_sources(fultst PRIVATE ${FULTST_SOURCES})
	target_include_directories(fultst PRIVATE "tst")
	target_link_libraries(fultst PRIVATE fulcmn lib_test)
	target_compile_options(fultst PRIVATE ${_compile_options})
	target_compile_definitions(fultst PRIVATE ${_compile_definitions})

	if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} VERSION_GREATER 3.7)
		set_target_properties(fultst PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_HOME_DIRECTORY}")
	endif()
endif()

if(FUL_TESTS_BUILDTIME)
	run_tests(runfultst fultst)
endif()

if(FUL_BUILD_BENCHMARKS)
	add_executable(fulbnc "")
	target_sources(fulbnc PRIVATE ${FULBNC_SOURCES})
	target_include_directories(fulbnc PRIVATE "bnc")
	target_link_libraries(fulbnc PRIVATE fulcmn lib_benchmark)
	target_compile_options(fulbnc PRIVATE ${_compile_options})
	target_compile_definitions(fulbnc PRIVATE ${_compile_definitions})

	if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} VERSION_GREATER 3.7)
		set_target_properties(fulbnc PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_HOME_DIRECTORY}")
	endif()

	if(TARGET runfultst)
		add_dependencies(fulbnc runfultst)
	endif()
endif()
