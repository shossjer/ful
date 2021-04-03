cmake_minimum_required(VERSION 3.5.1)

if(POLICY CMP0066)
	cmake_policy(SET CMP0066 NEW)
endif()

if(POLICY CMP0067)
	cmake_policy(SET CMP0067 NEW)
endif()

include(CMakeDependentOption)
include(dep/dep.cmake)

project(ful CXX)

# project setup

if(DEFINED CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Detected multi configuration build: ${CMAKE_CONFIGURATION_TYPES}")

	if(DEFINED CMAKE_BUILD_TYPE)
		message(WARNING "Both CMAKE_CONFIGURATION_TYPES and CMAKE_BUILD_TYPE are set! Please unset CMAKE_BUILD_TYPE to avoid confusion.")
	endif()
elseif(DEFINED CMAKE_BUILD_TYPE)
	if(CMAKE_BUILD_TYPE STREQUAL "")
		message(STATUS "Detected single configuration build")
	else()
		message(STATUS "Detected single configuration build: ${CMAKE_BUILD_TYPE}")
	endif()
else()
	message(FATAL_ERROR "Neither CMAKE_CONFIGURATION_TYPES or CMAKE_BUILD_TYPE are set!")
endif()

if(NOT CMAKE_CXX_STANDARD)
	set(CMAKE_CXX_STANDARD 11)
	message(STATUS "C++ standard ${CMAKE_CXX_STANDARD} (default)")
else()
	message(STATUS "C++ standard ${CMAKE_CXX_STANDARD}")
endif()
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# project information

message(STATUS "Using C++ compiler ${CMAKE_CXX_COMPILER}")
message(STATUS "System is ${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_PROCESSOR}")

# build dependencies

option(BUILD_CATCH "Build catch" ON)

if(BUILD_CATCH)
	dep_build(catch
		DOWNLOAD_STEP
		GIT_REPOSITORY "https://github.com/philsquared/Catch.git"
		GIT_TAG "v2.12.1"
		GIT_SHALLOW True

		CMAKE_OPTIONS
		-DCATCH_BUILD_TESTING=OFF
		-DCATCH_INSTALL_DOCS=OFF
		-DCATCH_INSTALL_HELPERS=OFF
		)
endif()

dep_package(catch ful_catch
	FILES
	catch2/catch.hpp
	)

# build settings

cmake_dependent_option(BUILD_TESTS "Build tests" ON "TARGET ful_catch" OFF)
add_library(lib_test INTERFACE)
if(BUILD_TESTS)
	target_link_libraries(lib_test INTERFACE ful_catch)
	target_compile_options(lib_test INTERFACE
		$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
			-Wno-sign-compare -Wno-sign-conversion>
		$<$<CXX_COMPILER_ID:Clang>:
			-Wno-self-move>
		$<$<CXX_COMPILER_ID:MSVC>:
			/wd4389 /wd4456>
	)
endif()

cmake_dependent_option(BUILD_BENCHMARKS "Build benchmarks" OFF "TARGET ful_catch" OFF)
add_library(lib_benchmark INTERFACE)
if(BUILD_BENCHMARKS)
	target_link_libraries(lib_benchmark INTERFACE ful_catch)
	target_compile_definitions(lib_benchmark INTERFACE CATCH_CONFIG_ENABLE_BENCHMARKING)
endif()

# build features

cmake_dependent_option(BUILD_ASMLIB "Build asmlib" ON "BUILD_BENCHMARKS" OFF)
if(BUILD_ASMLIB)
	dep_build(asmlib
		DOWNLOAD_STEP
		URL "https://www.agner.org/optimize/asmlib.zip"
		TIMEOUT 10

		INSTALL_STEP
		INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_if_different "\$\{_source_dir\}/asmlib.h" "\$\{_install_prefix\}/include"
		Linux_x86_64
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "\$\{_source_dir\}/libaelf64.a" "\$\{_install_prefix\}/lib"
		Windows_AMD64
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "\$\{_source_dir\}/libacof64.lib" "\$\{_install_prefix\}/lib"
		)
endif()

dep_package(asmlib ful_asmlib
	FILES
	asmlib.h

	LIBS
	Linux_x86_64 libaelf64.a
	Windows_AMD64 libacof64.lib
	)

if(TARGET ful_asmlib)
	target_link_libraries(lib_benchmark INTERFACE ful_asmlib)
	target_compile_definitions(lib_benchmark INTERFACE HAVE_ASMLIB)
endif()

# project options

# https://gcc.gnu.org/onlinedocs/gcc/x86-Options.html
# https://docs.microsoft.com/en-us/cpp/build/reference/arch-x64
option(CPUID_SSE "Assume SSE intrinsics are available" OFF)
option(CPUID_SSE2 "Assume SSE2 intrinsics are available" OFF)
option(CPUID_SSE3 "Assume SSE3 intrinsics are available" OFF)
option(CPUID_SSSE3 "Assume SSSE3 intrinsics are available" OFF)
option(CPUID_SSE4_1 "Assume SSE4.1 intrinsics are available" OFF)
option(CPUID_SSE4_2 "Assume SSE4.2 intrinsics are available" OFF)
option(CPUID_POPCNT "Assume POPCNT intrinsics are available" OFF)
option(CPUID_AVX "Assume AVX intrinsics are available" OFF)
option(CPUID_AVX2 "Assume AVX2 intrinsics are available" OFF)
option(CPUID_BMI "Assume BMI intrinsics are available" OFF)
option(CPUID_BMI2 "Assume BMI2 intrinsics are available" OFF)
option(CPUID_LZCNT "Assume LZCNT intrinsics are available" OFF)

if(CMAKE_CXX_COMPILER_ID EQUAL Clang OR CMAKE_CXX_COMPILER_ID EQUAL GNU)
	if(CMAKE_CXX_FLAGS MATCHES "-msse " OR CMAKE_CXX_FLAGS MATCHES "-msse$")
		set(CPUID_SSE ON CACHE BOOL "Assume SSE intrinsics are available" FORCE)
	endif()
	if(CMAKE_CXX_FLAGS MATCHES "-msse2 " OR CMAKE_CXX_FLAGS MATCHES "-msse2$")
		set(CPUID_SSE2 ON CACHE BOOL "Assume SSE2 intrinsics are available" FORCE)
	endif()
	if(CMAKE_CXX_FLAGS MATCHES "-msse3 " OR CMAKE_CXX_FLAGS MATCHES "-msse3$")
		set(CPUID_SSE3 ON CACHE BOOL "Assume SSE3 intrinsics are available" FORCE)
	endif()
	if(CMAKE_CXX_FLAGS MATCHES "-mssse3 " OR CMAKE_CXX_FLAGS MATCHES "-mssse3$")
		set(CPUID_SSSE3 ON CACHE BOOL "Assume SSSE3 intrinsics are available" FORCE)
	endif()
	if(CMAKE_CXX_FLAGS MATCHES "-msse4.1 " OR CMAKE_CXX_FLAGS MATCHES "-msse4.1$")
		set(CPUID_SSE4_1 ON CACHE BOOL "Assume SSE4_1 intrinsics are available" FORCE)
	endif()
	if(CMAKE_CXX_FLAGS MATCHES "-msse4.2 " OR CMAKE_CXX_FLAGS MATCHES "-msse4.2$")
		set(CPUID_SSE4_2 ON CACHE BOOL "Assume SSE4_2 intrinsics are available" FORCE)
	endif()
	if(CMAKE_CXX_FLAGS MATCHES "-mpopcnt " OR CMAKE_CXX_FLAGS MATCHES "-mpopcnt$")
		set(CPUID_POPCNT ON CACHE BOOL "Assume POPCNT intrinsics are available" FORCE)
	endif()
	if(CMAKE_CXX_FLAGS MATCHES "-mavx " OR CMAKE_CXX_FLAGS MATCHES "-mavx$")
		set(CPUID_AVX ON CACHE BOOL "Assume AVX intrinsics are available" FORCE)
	endif()
	if(CMAKE_CXX_FLAGS MATCHES "-mavx2 " OR CMAKE_CXX_FLAGS MATCHES "-mavx2$")
		set(CPUID_AVX2 ON CACHE BOOL "Assume AVX2 intrinsics are available" FORCE)
	endif()
	if(CMAKE_CXX_FLAGS MATCHES "-mbmi " OR CMAKE_CXX_FLAGS MATCHES "-mbmi$")
		set(CPUID_BMI ON CACHE BOOL "Assume BMI intrinsics are available" FORCE)
	endif()
	if(CMAKE_CXX_FLAGS MATCHES "-mbmi2 " OR CMAKE_CXX_FLAGS MATCHES "-mbmi2$")
		set(CPUID_BMI2 ON CACHE BOOL "Assume BMI2 intrinsics are available" FORCE)
	endif()
	if(CMAKE_CXX_FLAGS MATCHES "-mlzcnt " OR CMAKE_CXX_FLAGS MATCHES "-mlzcnt$")
		set(CPUID_LZCNT ON CACHE BOOL "Assume LZCNT intrinsics are available" FORCE)
	endif()
elseif(CMAKE_CXX_COMPILER_ID EQUAL MSVC)
	if(CMAKE_CXX_FLAGS MATCHES "/arch:SSE " OR CMAKE_CXX_FLAGS MATCHES "/arch:SSE$")
		set(CPUID_SSE ON CACHE BOOL "Assume SSE intrinsics are available" FORCE)
	endif()
	if(CMAKE_CXX_FLAGS MATCHES "/arch:SSE2 " OR CMAKE_CXX_FLAGS MATCHES "/arch:SSE2$")
		set(CPUID_SSE ON CACHE BOOL "Assume SSE intrinsics are available" FORCE)
		set(CPUID_SSE2 ON CACHE BOOL "Assume SSE2 intrinsics are available" FORCE)
	endif()
	if(CMAKE_CXX_FLAGS MATCHES "/arch:AVX " OR CMAKE_CXX_FLAGS MATCHES "/arch:AVX$")
		set(CPUID_SSE ON CACHE BOOL "Assume SSE intrinsics are available" FORCE)
		set(CPUID_SSE2 ON CACHE BOOL "Assume SSE2 intrinsics are available" FORCE)
		set(CPUID_SSE3 ON CACHE BOOL "Assume SSE3 intrinsics are available" FORCE)
		set(CPUID_SSSE3 ON CACHE BOOL "Assume SSSE3 intrinsics are available" FORCE)
		set(CPUID_SSE4_1 ON CACHE BOOL "Assume SSE4_1 intrinsics are available" FORCE)
		set(CPUID_SSE4_2 ON CACHE BOOL "Assume SSE4_2 intrinsics are available" FORCE)
		set(CPUID_AVX ON CACHE BOOL "Assume AVX intrinsics are available" FORCE)
	endif()
	if(CMAKE_CXX_FLAGS MATCHES "/arch:AVX2 " OR CMAKE_CXX_FLAGS MATCHES "/arch:AVX2$")
		set(CPUID_SSE ON CACHE BOOL "Assume SSE intrinsics are available" FORCE)
		set(CPUID_SSE2 ON CACHE BOOL "Assume SSE2 intrinsics are available" FORCE)
		set(CPUID_SSE3 ON CACHE BOOL "Assume SSE3 intrinsics are available" FORCE)
		set(CPUID_SSSE3 ON CACHE BOOL "Assume SSSE3 intrinsics are available" FORCE)
		set(CPUID_SSE4_1 ON CACHE BOOL "Assume SSE4_1 intrinsics are available" FORCE)
		set(CPUID_SSE4_2 ON CACHE BOOL "Assume SSE4_2 intrinsics are available" FORCE)
		set(CPUID_AVX ON CACHE BOOL "Assume AVX intrinsics are available" FORCE)
		set(CPUID_AVX2 ON CACHE BOOL "Assume AVX2 intrinsics are available" FORCE)
	endif()
endif()

option(CONFTIME_CPUID "Probe CPUID at configure-time, setting CPUID_* if not set already" ON)
option(RUNTIME_CPUID "Probe CPUID at runtime" OFF)

if(CONFTIME_CPUID AND NOT CONFTIME_CPUID_RESULT_RUN EQUAL 0)
	try_run(
		CONFTIME_CPUID_RESULT_RUN
		CONFTIME_CPUID_RESULT_COMPILE

		"${PROJECT_BINARY_DIR}/probe/cpuid"
		"${PROJECT_SOURCE_DIR}/.probe/cpuid.cpp"

		CMAKE_FLAGS
		"-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
		"-DINCLUDE_DIRECTORIES=${PROJECT_SOURCE_DIR}/code/src"

		RUN_OUTPUT_VARIABLE _cpuid_output
		)

	if(CONFTIME_CPUID_RESULT_COMPILE AND CONFTIME_CPUID_RESULT_RUN EQUAL 0)
		message(STATUS "Probe CPUID at configure-time - ${_cpuid_output}")

		if("SSE" IN_LIST _cpuid_output)
			set(CPUID_SSE ON CACHE BOOL "Assume SSE intrinsics are available" FORCE)
		endif()
		if("SSE2" IN_LIST _cpuid_output)
			set(CPUID_SSE2 ON CACHE BOOL "Assume SSE2 intrinsics are available" FORCE)
		endif()
		if("SSE3" IN_LIST _cpuid_output)
			set(CPUID_SSE3 ON CACHE BOOL "Assume SSE3 intrinsics are available" FORCE)
		endif()
		if("SSSE3" IN_LIST _cpuid_output)
			set(CPUID_SSSE3 ON CACHE BOOL "Assume SSSE3 intrinsics are available" FORCE)
		endif()
		if("SSE4_1" IN_LIST _cpuid_output)
			set(CPUID_SSE4_1 ON CACHE BOOL "Assume SSE4.1 intrinsics are available" FORCE)
		endif()
		if("SSE4_2" IN_LIST _cpuid_output)
			set(CPUID_SSE4_2 ON CACHE BOOL "Assume SSE4.2 intrinsics are available" FORCE)
		endif()
		if("POPCNT" IN_LIST _cpuid_output)
			set(CPUID_POPCNT ON CACHE BOOL "Assume POPCNT intrinsics are available" FORCE)
		endif()
		if("AVX" IN_LIST _cpuid_output)
			set(CPUID_AVX ON CACHE BOOL "Assume AVX intrinsics are available" FORCE)
		endif()
		if("AVX2" IN_LIST _cpuid_output)
			set(CPUID_AVX2 ON CACHE BOOL "Assume AVX2 intrinsics are available" FORCE)
		endif()
		if("BMI" IN_LIST _cpuid_output)
			set(CPUID_BMI ON CACHE BOOL "Assume BMI intrinsics are available" FORCE)
		endif()
		if("BMI2" IN_LIST _cpuid_output)
			set(CPUID_BMI2 ON CACHE BOOL "Assume BMI2 intrinsics are available" FORCE)
		endif()
		if("LZCNT" IN_LIST _cpuid_output)
			set(CPUID_LZCNT ON CACHE BOOL "Assume LZCNT intrinsics are available" FORCE)
		endif()
	else()
		message(WARNING "Probe CPUID at configure-time - failed to run successfully")
	endif()
endif()

add_library(lib_cpuid INTERFACE)
if(RUNTIME_CPUID)
	target_compile_definitions(lib_cpuid INTERFACE RUNTIME_CPUID)
endif()
if(CPUID_SSE)
	target_compile_options(lib_cpuid INTERFACE
		$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
			-msse>
		$<$<CXX_COMPILER_ID:MSVC>:
			/arch:SSE>
		)
	target_compile_definitions(lib_cpuid INTERFACE
		$<$<CXX_COMPILER_ID:MSVC>:
			__SSE__>
		)
endif()
if(CPUID_SSE2)
	target_compile_options(lib_cpuid INTERFACE
		$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
			-msse2>
		$<$<CXX_COMPILER_ID:MSVC>:
			/arch:SSE2>
		)
	target_compile_definitions(lib_cpuid INTERFACE
		$<$<CXX_COMPILER_ID:MSVC>:
			__SSE2__>
		)
endif()
if(CPUID_SSE3)
	target_compile_options(lib_cpuid INTERFACE
		$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
			-msse3>
		)
	target_compile_definitions(lib_cpuid INTERFACE
		$<$<CXX_COMPILER_ID:MSVC>:
			__SSE3__>
		)
endif()
if(CPUID_SSSE3)
	target_compile_options(lib_cpuid INTERFACE
		$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
			-mssse3>
		)
	target_compile_definitions(lib_cpuid INTERFACE
		$<$<CXX_COMPILER_ID:MSVC>:
			__SSSE3__>
		)
endif()
if(CPUID_SSE4_1)
	target_compile_options(lib_cpuid INTERFACE
		$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
			-msse4.1>
		)
	target_compile_definitions(lib_cpuid INTERFACE
		$<$<CXX_COMPILER_ID:MSVC>:
			__SSE4_1__>
		)
endif()
if(CPUID_SSE4_2)
	target_compile_options(lib_cpuid INTERFACE
		$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
			-msse4.2>
		)
	target_compile_definitions(lib_cpuid INTERFACE
		$<$<CXX_COMPILER_ID:MSVC>:
			__SSE4_2__>
		)
endif()
if(CPUID_POPCNT)
	target_compile_options(lib_cpuid INTERFACE
		$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
			-mpopcnt>
		)
	target_compile_definitions(lib_cpuid INTERFACE
		$<$<CXX_COMPILER_ID:MSVC>:
			__POPCNT__>
		)
endif()
if(CPUID_AVX)
	target_compile_options(lib_cpuid INTERFACE
		$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
			-mavx>
		$<$<CXX_COMPILER_ID:MSVC>:
			/arch:AVX>
		)
endif()
if(CPUID_AVX2)
	target_compile_options(lib_cpuid INTERFACE
		$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
			-mavx2>
		$<$<CXX_COMPILER_ID:MSVC>:
			/arch:AVX2>
		)
endif()
if(CPUID_BMI)
	target_compile_options(lib_cpuid INTERFACE
		$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
			-mbmi>
		)
	target_compile_definitions(lib_cpuid INTERFACE
		$<$<CXX_COMPILER_ID:MSVC>:
			__BMI__>
		)
endif()
if(CPUID_BMI2)
	target_compile_options(lib_cpuid INTERFACE
		$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
			-mbmi2>
		)
	target_compile_definitions(lib_cpuid INTERFACE
		$<$<CXX_COMPILER_ID:MSVC>:
			__BMI2__>
		)
endif()
if(CPUID_LZCNT)
	target_compile_options(lib_cpuid INTERFACE
		$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
			-mlzcnt>
		)
	target_compile_definitions(lib_cpuid INTERFACE
		$<$<CXX_COMPILER_ID:MSVC>:
			__LZCNT__>
		)
endif()

add_subdirectory(code)
