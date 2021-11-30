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

if(MSVC)
	enable_language(ASM_MASM)
endif()

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
elseif(CMAKE_CXX_STANDARD LESS 11)
	message(FATAL_ERROR "Require C++ standard >= 11")
else()
	message(STATUS "C++ standard ${CMAKE_CXX_STANDARD}")
endif()
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

get_property(_dep_directory_set DIRECTORY PROPERTY "DEP_DIRECTORY" SET)
if(NOT _dep_directory_set)
	set_property(DIRECTORY PROPERTY "DEP_DIRECTORY" "${CMAKE_SOURCE_DIR}/dep/${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}")
endif()

# project information

message(STATUS "Using C++ compiler ${CMAKE_CXX_COMPILER}")
message(STATUS "System is ${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_PROCESSOR}")

# build dependencies

macro(_package_catch)
	dep_package(catch ful_catch
		${ARGV}

		FILES
		catch2/catch.hpp
		)
endmacro()

option(FUL_BUILD_CATCH "Build catch" ON)
if(FUL_BUILD_CATCH)
	_package_catch(IGNORE_SYSTEM)

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

_package_catch()

# build settings

add_library(ful_library INTERFACE)
add_library(ful_common INTERFACE)
target_compile_options(ful_common INTERFACE
	$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
		-march=native>
	)

cmake_dependent_option(FUL_BUILD_TESTS "Build tests" ON "TARGET ful_catch" OFF)
add_library(ful_test INTERFACE)
if(FUL_BUILD_TESTS)
	target_link_libraries(ful_test INTERFACE ful_catch)
	target_compile_options(ful_test INTERFACE
		$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
			-Wno-sign-compare -Wno-sign-conversion>
		$<$<CXX_COMPILER_ID:Clang>:
			-Wno-self-move>
		$<$<CXX_COMPILER_ID:MSVC>:
			/wd4389 /wd4456>
	)
endif()

cmake_dependent_option(FUL_TESTS_BUILDTIME "Run tests as part of the build process." ON "FUL_BUILD_TESTS" OFF)

cmake_dependent_option(FUL_BUILD_BENCHMARKS "Build benchmarks" OFF "TARGET ful_catch" OFF)
add_library(ful_benchmark INTERFACE)
if(FUL_BUILD_BENCHMARKS)
	target_link_libraries(ful_benchmark INTERFACE ful_catch)
	target_compile_definitions(ful_benchmark INTERFACE CATCH_CONFIG_ENABLE_BENCHMARKING)
endif()

# build features

# https://www.agner.org/optimize/#asmlib
macro(_package_asmlib)
	dep_package(asmlib ful_asmlib
		${ARGV}

		FILES
		asmlib.h

		LIBS
		Linux_x86_64 libaelf64.a
		Windows_AMD64 libacof64.lib
		)
endmacro()

cmake_dependent_option(FUL_BUILD_ASMLIB "Build asmlib" ON "FUL_BUILD_BENCHMARKS" OFF)
if(FUL_BUILD_ASMLIB)
	_package_asmlib(IGNORE_SYSTEM)

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

_package_asmlib()
if(TARGET ful_asmlib)
	target_link_libraries(ful_benchmark INTERFACE ful_asmlib)
	target_compile_definitions(ful_benchmark INTERFACE HAVE_ASMLIB)
endif()

# https://github.com/electronicarts/EAStdC
macro(_package_eastdc)
	dep_package(eastdc-latest ful_eastdc
		SUPPORTS_DEBUG
		${ARGV}

		FILES
		EAStdC/EAString.h

		LIBS
		EAStdC
		)
endmacro()

cmake_dependent_option(FUL_BUILD_EASTDC "Build eastdc" ON "FUL_BUILD_BENCHMARKS AND POLICY CMP0097" OFF)
if(FUL_BUILD_EASTDC)
	_package_eastdc(IGNORE_SYSTEM)

	# note eastdc has a nasty recursive dependency between its submodules
	# so we have to hack around it by not fetching the submodules when
	# cloning but instead postpone it and make an explicit request to
	# fetch the submodules later; this is why we require POLICY CMP0097
	# (available since version 3.14)
	#
	# https://gitlab.kitware.com/cmake/cmake/-/issues/19832
	#
	# furthermore, the latest version of the library is missing an
	# install target and recursive calls to its dependencies, so we use
	# this DIY version of the library instead
	dep_build(eastdc-latest
		POLICY_CMP0097
		FIND_GIT
		SUPPORTS_DEBUG

		DOWNLOAD_STEP
		GIT_REPOSITORY "https://github.com/shossjer/EAStdC.git"
		GIT_TAG "install"
		GIT_SUBMODULES ""
		GIT_SHALLOW True

		UPDATE_STEP
		UPDATE_COMMAND "${CMAKE_COMMAND}" -E chdir "\$\{_source_dir\}" "\$\{GIT_EXECUTABLE\}" submodule update --init "test/packages/EABase" "test/packages/EAAssert" "test/packages/EAThread"
		COMMAND "${CMAKE_COMMAND}" -E touch "\$\{_source_dir\}/CommonCppFlags"
		COMMAND "${CMAKE_COMMAND}" -E touch "\$\{_source_dir\}/test/packages/EAAssert/CommonCppFlags"
		COMMAND "${CMAKE_COMMAND}" -E touch "\$\{_source_dir\}/test/packages/EAThread/CommonCppFlags"

		CMAKE_OPTIONS
		-DEASTDC_BUILD_TESTS=OFF
		)
endif()

_package_eastdc()
if(TARGET ful_eastdc)
	target_link_libraries(ful_benchmark INTERFACE ful_eastdc)
	target_compile_definitions(ful_benchmark INTERFACE HAVE_EASTDC)
endif()

# https://github.com/electronicarts/EASTL
macro(_package_eastl)
	dep_package(eastl-latest ful_eastl
		SUPPORTS_DEBUG
		${ARGV}

		FILES
		EASTL/string.h

		LIBS
		EASTL
		)
endmacro()

cmake_dependent_option(FUL_BUILD_EASTL "Build eastl" ON "FUL_BUILD_BENCHMARKS AND POLICY CMP0097" OFF)
if(FUL_BUILD_EASTL)
	_package_eastl(IGNORE_SYSTEM)

	# note eastl has a nasty recursive dependency between its
	# submodules so we have to hack around it by not fetching the
	# submodules when cloning but instead postpone it and make an
	# explicit request to fetch the submodules later (or in this case
	# only the one submodule we actually need); this is why we require
	# POLICY CMP0097 (available since version 3.14)
	#
	# https://gitlab.kitware.com/cmake/cmake/-/issues/19832
	#
	# furthermore, the latest version of the library is missing an
	# install target so we use this DIY version of the library instead
	# (until it gets merged or fixed some other way)
	dep_build(eastl-latest
		POLICY_CMP0097
		FIND_GIT
		SUPPORTS_DEBUG

		DOWNLOAD_STEP
		GIT_REPOSITORY "https://github.com/shossjer/EASTL.git"
		GIT_TAG "install"
		GIT_SUBMODULES ""
		GIT_SHALLOW True

		UPDATE_STEP
		UPDATE_COMMAND "${CMAKE_COMMAND}" -E chdir "\$\{_source_dir\}" "\$\{GIT_EXECUTABLE\}" submodule update --init "test/packages/EABase"

		CMAKE_OPTIONS
		-DEASTL_BUILD_BENCHMARK=OFF
		-DEASTL_BUILD_TESTS=OFF
		)
endif()

_package_eastl()
if(TARGET ful_eastl)
	target_link_libraries(ful_benchmark INTERFACE ful_eastl)
	target_compile_definitions(ful_benchmark INTERFACE HAVE_EASTL)
endif()

# http://site.icu-project.org/
macro(_package_icu4c)
	dep_package(icu4c ful_icu4c
		${ARGV}

		FILES
		unicode/ucnv.h

		LIBS
		Linux_x86_64 icuuc icudata
		Windows_AMD64 icuuc.lib
		)
endmacro()

cmake_dependent_option(FUL_BUILD_ICU4C "Build icu4c" ON "FUL_BUILD_BENCHMARKS" OFF)
if(FUL_BUILD_ICU4C)
	_package_icu4c(IGNORE_SYSTEM)

	dep_build(icu4c
		DOWNLOAD_STEP
		Linux_x86_64
		URL "https://github.com/unicode-org/icu/releases/download/release-69-1/icu4c-69_1-src.tgz"
		TIMEOUT 20
		Windows_AMD64
		URL "https://github.com/unicode-org/icu/releases/download/release-69-1/icu4c-69_1-Win64-MSVC2019.zip"
		TIMEOUT 20

		CONFIGURE_STEP
		Linux_x86_64
		CONFIGURE_COMMAND "\$\{_source_dir\}/source/configure" "--prefix=\$\{_install_prefix\}" --enable-static --disable-shared --disable-icu-config --disable-extras --disable-icuio --disable-tools --disable-tests --disable-samples

		INSTALL_STEP
		Windows_AMD64
		INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory "\$\{_source_dir\}/include" "\$\{_install_prefix\}/include"
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "\$\{_source_dir\}/lib64/icuuc.lib" "\$\{_install_prefix\}/lib"
		COMMAND ${CMAKE_COMMAND} -E make_directory "\$\{_install_prefix\}/bin"
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "\$\{_source_dir\}/bin64/icuuc69.dll" "\$\{_source_dir\}/bin64/icudt69.dll" "\$\{_install_prefix\}/bin"
		)
endif()

_package_icu4c()
if(TARGET ful_icu4c)
	target_link_libraries(ful_benchmark INTERFACE ful_icu4c)
	if(UNIX)
		# todo dl is needed
		# todo pthread is needed for some reason - https://bugzilla.redhat.com/show_bug.cgi?id=1954178
		target_link_libraries(ful_benchmark INTERFACE dl pthread)
	endif()
	target_compile_definitions(ful_benchmark INTERFACE HAVE_ICU4C)
endif()

# https://www.gnu.org/software/libiconv/
macro(_package_libiconv)
	dep_package(libiconv ful_libiconv
		${ARGV}

		FILES
		iconv.h

		LIBS
		iconv
		)
endmacro()

cmake_dependent_option(FUL_BUILD_LIBICONV "Build libiconv" ON "FUL_BUILD_BENCHMARKS AND UNIX" OFF)
if(FUL_BUILD_LIBICONV)
	_package_libiconv(IGNORE_SYSTEM)

	dep_build(libiconv
		DOWNLOAD_STEP
		URL "https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.16.tar.gz"
		TIMEOUT 20

		CONFIGURE_STEP
		CONFIGURE_COMMAND "\$\{_source_dir\}/configure" "--prefix=\$\{_install_prefix\}" --enable-static --disable-shared

		BUILD_STEP
		BUILD_COMMAND make install
		)
endif()

_package_libiconv()
if(TARGET ful_libiconv)
	target_link_libraries(ful_benchmark INTERFACE ful_libiconv)
	target_compile_definitions(ful_benchmark INTERFACE HAVE_LIBICONV)
endif()

# https://www.gnu.org/software/libunistring/
macro(_package_libunistring)
	dep_package(libunistring ful_libunistring
		${ARGV}

		FILES
		unistr.h

		LIBS
		unistring
		)
endmacro()

cmake_dependent_option(FUL_BUILD_LIBUNISTRING "Build libunistring" ON "FUL_BUILD_BENCHMARKS AND UNIX" OFF)
if(FUL_BUILD_LIBUNISTRING)
	_package_libunistring(IGNORE_SYSTEM)

	dep_build(libunistring
		DOWNLOAD_STEP
		URL "https://ftp.gnu.org/gnu/libunistring/libunistring-0.9.10.tar.gz"
		TIMEOUT 10

		CONFIGURE_STEP
		CONFIGURE_COMMAND "\$\{_source_dir\}/configure" "--prefix=\$\{_install_prefix\}" --disable-shared

		BUILD_STEP
		BUILD_COMMAND make install
		)
endif()

_package_libunistring()
if(TARGET ful_libunistring)
	target_link_libraries(ful_benchmark INTERFACE ful_libunistring)
	target_compile_definitions(ful_benchmark INTERFACE HAVE_LIBUNISTRING)
endif()

# https://github.com/DuffsDevice/tiny-utf8
macro(_package_tinyutf8)
	dep_package(tinyutf8 ful_tinyutf8
		${ARGV}

		FILES
		tinyutf8/tinyutf8.h
		)
endmacro()

cmake_dependent_option(FUL_BUILD_TINYUTF8 "Build tiny-utf8" ON "FUL_BUILD_BENCHMARKS" OFF)
if(FUL_BUILD_TINYUTF8)
	_package_tinyutf8(IGNORE_SYSTEM)

	dep_build(tinyutf8
		DOWNLOAD_STEP
		URL "https://github.com/DuffsDevice/tiny-utf8/archive/refs/tags/v4.3.2.tar.gz"
		TIMEOUT 10

		CMAKE_OPTIONS
		-DTINYUTF8_BUILD_TESTING=OFF
		-DTINYUTF8_BUILD_DOC=OFF
		)
endif()

_package_tinyutf8()
if(TARGET ful_tinyutf8)
	target_link_libraries(ful_benchmark INTERFACE ful_tinyutf8)
	target_compile_definitions(ful_benchmark INTERFACE HAVE_TINYUTF8)
endif()

# https://github.com/JuliaStrings/utf8proc
macro(_package_utf8proc)
	dep_package(utf8proc ful_utf8proc
		SUPPORTS_DEBUG
		${ARGV}

		FILES
		utf8proc.h

		LIBS
		Linux_x86_64 utf8proc
		Windows_AMD64 utf8proc_static.lib
		)
endmacro()

cmake_dependent_option(FUL_BUILD_UTF8PROC "Build utf8proc" ON "FUL_BUILD_BENCHMARKS" OFF)
if(FUL_BUILD_UTF8PROC)
	_package_utf8proc(IGNORE_SYSTEM)

	dep_build(utf8proc
		SUPPORTS_DEBUG

		DOWNLOAD_STEP
		URL "https://github.com/JuliaStrings/utf8proc/archive/refs/tags/v2.6.1.tar.gz"
		TIMEOUT 10

		CMAKE_OPTIONS
		-DBUILD_SHARED_LIBS=OFF
		)
endif()

_package_utf8proc()
if(TARGET ful_utf8proc)
	target_link_libraries(ful_benchmark INTERFACE ful_utf8proc)
	# todo only define UTF8PROC_STATIC when static lib is found
	target_compile_definitions(ful_benchmark INTERFACE HAVE_UTF8PROC UTF8PROC_STATIC)
endif()


# https://github.com/nemtrif/utfcpp
macro(_package_utfcpp)
	dep_package(utfcpp ful_utfcpp
		${ARGV}

		FILES
		utf8cpp/utf8.h
		)
endmacro()

cmake_dependent_option(FUL_BUILD_UTFCPP "Build utfcpp" ON "FUL_BUILD_BENCHMARKS" OFF)
if(FUL_BUILD_UTFCPP)
	_package_utfcpp(IGNORE_SYSTEM)

	dep_build(utfcpp
		DOWNLOAD_STEP
		URL "https://github.com/nemtrif/utfcpp/archive/refs/tags/v3.1.2.tar.gz"
		TIMEOUT 10

		CMAKE_OPTIONS
		-DUTF8_TESTS=OFF
		-DUTF8_INSTALL=ON
		-DUTF8_SAMPLES=OFF
		)
endif()

_package_utfcpp()
if(TARGET ful_utfcpp)
	target_link_libraries(ful_benchmark INTERFACE ful_utfcpp)
	target_compile_definitions(ful_benchmark INTERFACE HAVE_UTFCPP)
endif()

# project options

# todo this is a copy-paste from above, can this be solved better?
if(TARGET ful_libiconv)
	message(STATUS "Common buffer conversion using libiconv")
	target_link_libraries(ful_common INTERFACE ful_libiconv)
	target_compile_definitions(ful_common INTERFACE HAVE_LIBICONV)
elseif(TARGET ful_icu4c)
	message(STATUS "Common buffer conversion using icu4c")
	target_link_libraries(ful_common INTERFACE ful_icu4c)
	if(UNIX)
		# todo dl is needed
		# todo pthread is needed for some reason - https://bugzilla.redhat.com/show_bug.cgi?id=1954178
		target_link_libraries(ful_common INTERFACE dl pthread)
	endif()
	target_compile_definitions(ful_common INTERFACE HAVE_ICU4C)
else()
	set(_test_arguments "\"~[common buffer conversion]\"")
endif()

add_subdirectory(code)
