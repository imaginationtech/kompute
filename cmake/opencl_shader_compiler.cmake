function(opencl_compile_shader)
     find_program(CLSPV_PATH NAMES clspv)
     if(CLSPV_PATH STREQUAL "CLSPV_PATH-NOTFOUND")
          message(FATAL_ERROR "clspv not found.")
          return()
     endif()

     set(oneValueArgs INFILE OUTFILE NAMESPACE RELATIVE_PATH VARIABLE_NAME)
     set(multiValueArgs ARGS)
     cmake_parse_arguments(SHADER_COMPILE "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
     set(SHADER_COMPILE_INFILE_FULL "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER_COMPILE_INFILE}")
     set(SHADER_COMPILE_SPV_FILE_FULL "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_COMPILE_INFILE}.spv")
     set(SHADER_COMPILE_HEADER_FILE_FULL "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_COMPILE_OUTFILE}")

     if(NOT SHADER_COMPILE_RELATIVE_PATH)
          set(SHADER_COMPILE_RELATIVE_PATH "${PROJECT_SOURCE_DIR}/cmake")
     endif()
    
     # .cl -> .sh
     add_custom_command(OUTPUT "${SHADER_COMPILE_SPV_FILE_FULL}"
                        COMMAND "${CLSPV_PATH}"
                        ARGS ${SHADER_COMPILE_ARGS}
                             "${SHADER_COMPILE_INFILE_FULL}"
                             "-o"
                             "${SHADER_COMPILE_SPV_FILE_FULL}"
                        COMMENT "Compile OpenCL compute shader from file '${SHADER_COMPILE_INFILE_FULL}' to '${SHADER_COMPILE_SPV_FILE_FULL}'."
                        MAIN_DEPENDENCY "${SHADER_COMPILE_INFILE_FULL}")

     # Check if big or little endian
     include (TestBigEndian)
     TEST_BIG_ENDIAN(IS_BIG_ENDIAN)

     # .spv -> .hpp
     add_custom_command(OUTPUT "${SHADER_COMPILE_HEADER_FILE_FULL}"
                        COMMAND ${CMAKE_COMMAND}
                        ARGS "-DINPUT_SHADER_FILE=${SHADER_COMPILE_SPV_FILE_FULL}"
                             "-DOUTPUT_HEADER_FILE=${SHADER_COMPILE_HEADER_FILE_FULL}"
                             "-DHEADER_NAMESPACE=${SHADER_COMPILE_NAMESPACE}"
                             "-DIS_BIG_ENDIAN=${IS_BIG_ENDIAN}"
                             "-DVARIABLE_NAME=${SHADER_COMPILE_VARIABLE_NAME}"
                             "-P"
                             "${SHADER_COMPILE_RELATIVE_PATH}/bin_file_to_header.cmake"
                        WORKING_DIRECTORY "${SHADER_COMPILE_RELATIVE_PATH}"
                        COMMENT "Converting compiled shader '${SHADER_COMPILE_SPV_FILE_FULL}' to header file '${SHADER_COMPILE_HEADER_FILE_FULL}'."
                        MAIN_DEPENDENCY "${SHADER_COMPILE_SPV_FILE_FULL}")
endfunction()
