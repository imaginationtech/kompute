function(opencl_compile_shader)
     find_program(CLSPV_PATH NAMES clspv)
     if(CLSPV_PATH STREQUAL "CLSPV_PATH-NOTFOUND")
          message(FATAL_ERROR "clspv not found.")
          return()
     endif()

     cmake_parse_arguments(SHADER_COMPILE "" "INFILE;OUTFILE;NAMESPACE;RELATIVE_PATH" "" ${ARGN})
     set(SHADER_COMPILE_INFILE_FULL "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER_COMPILE_INFILE}")
     set(SHADER_COMPILE_HEADER_FILE_FULL "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_COMPILE_OUTFILE}")

     if(NOT SHADER_COMPILE_RELATIVE_PATH)
          set(SHADER_COMPILE_RELATIVE_PATH "${PROJECT_SOURCE_DIR}/cmake")
     endif()
    
     # .cl -> .sh
     add_custom_command(OUTPUT "${SHADER_COMPILE_HEADER_FILE_FULL}"
                        COMMAND "${CLSPV_PATH}"
                        ARGS "-mfmt=c"
                             "${SHADER_COMPILE_INFILE_FULL}"
                             "-o"
                             "${SHADER_COMPILE_HEADER_FILE_FULL}"
                        COMMENT "Compile OpenCL compute shader from file '${SHADER_COMPILE_INFILE_FULL}' to '${SHADER_COMPILE_SPV_FILE_FULL}'."
                        MAIN_DEPENDENCY "${SHADER_COMPILE_INFILE_FULL}")
endfunction()
