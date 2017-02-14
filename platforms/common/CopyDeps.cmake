include(GetPrerequisites)
get_prerequisites("${BINARY}" DEPENDENCIES 1 1 "" "${DIRS}")

foreach(DEPENDENCY_FILE ${DEPENDENCIES})
	gp_resolve_item("${BINARY}" "${DEPENDENCY_FILE}" "" "${DIRS}" resolved_file)
	if(EXISTS ${resolved_file})
		execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different "${resolved_file}" "${DESTINATION}")
	endif()
endforeach()

