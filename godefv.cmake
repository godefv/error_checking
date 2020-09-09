# compile flags
if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP6 " ) # multi-threaded build
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4503 " ) # disable unreasonable MSVC warning "decorated name length exceeded"
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__STDC_LIMIT_MACROS " ) # visual studio sometimes needs this to compile
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS" ) # disable visual studio silly warnings
endif()

set(GCC_WARNING_FLAGS
    -Werror
    -Wall
    -Wextra
    -Wredundant-decls
    -Wcast-align
    -Wmissing-declarations
    -Wmissing-include-dirs
    -Wswitch-enum
    -Wswitch-default
    -Winvalid-pch
    -Wredundant-decls
    -Wformat=2
    -Wmissing-format-attribute
    -Wformat-nonliteral
    -flto
    -Wodr
)

# utility functions
function(color_message color msg)
	execute_process(COMMAND ${CMAKE_COMMAND} -E env CLICOLOR_FORCE=1 ${CMAKE_COMMAND} -E cmake_echo_color --${color} ${ARGN} "${msg}")
endfunction(color_message)

# install functions
function(install_headers destination)
	foreach(HEADER ${ARGN})
		get_filename_component( dir "${HEADER}" DIRECTORY )
		install(FILES ${HEADER} DESTINATION ${destination}/${dir} )
	endforeach(HEADER)
endfunction()

function(install_pdb target destination)
	# Windows only
	if(MSVC)
		# enable pdb
		target_compile_options(${target} PRIVATE /Zi)
		# set build directory for pdb to be the same as .lib
		set_target_properties(${target} PROPERTIES COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
		# install .pdb to destination
		install(
			FILES $<TARGET_FILE_DIR:${target}>/${target}.pdb
			DESTINATION ${destination} OPTIONAL
			)
	endif()
endfunction()

# add binaries and tests functions
function(godefv_add_module module_path)
	string(REPLACE "/" "_" module_name "${module_path}")
	set(module_target "godefv_${module_name}")
	set(module_sources ${ARGN})

	file(GLOB_RECURSE ${module_target}_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")

	# add_library
	color_message(cyan "adding module ${module_target}" --no-newline )
	if(ARGN MATCHES "^$")
		color_message(blue " (header only)")
		add_library(${module_target} INTERFACE )
		#target_sources(${module_target} INTERFACE ${${module_target}_headers})
	else()
		color_message(blue "")
		add_library(${module_target} ${module_sources} ${${module_target}_headers})
		# be nice with visual studio users 
		set_target_properties(${module_target} PROPERTIES FOLDER ${PROJECT_NAME}/${module_path})
		install_pdb(${module_target} lib)
	endif()

	# install
	install(TARGETS ${module_target}
		EXPORT ${PROJECT_NAME}
		RUNTIME DESTINATION lib OPTIONAL
		LIBRARY DESTINATION lib OPTIONAL
		ARCHIVE DESTINATION lib OPTIONAL
	)
	install_headers(include/${module_path} ${${module_target}_headers})
endfunction()

function(godefv_add_test module_path unit)
	string(REPLACE "/" "_" module_name "${module_path}")
	set(module_target "godefv_${module_name}")

	string(REPLACE "/" "_" test_target "${module_path}/${unit}")
	set(test_target "test_${test_target}")
	
	add_executable(${test_target} "${unit}.test.cpp")
	target_link_libraries(${test_target} ${module_target} ${ARGN})
	target_compile_options(${test_target} PRIVATE ${GCC_WARNING_FLAGS})
	set_target_properties(${test_target} PROPERTIES FOLDER ${PROJECT_NAME}/${module_path}/unit_test)
	if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
		add_test(${module_path}/${unit} ${test_target})
	endif()
	if(TEST_INSTALL_DIRECTORY)
		install(TARGETS ${test_target} DESTINATION ${TEST_INSTALL_DIRECTORY} OPTIONAL)
	endif()
endfunction()

