
# This function copys a directory next to the executable.
# On OS X the directory is created inside the Resources folder of the application bundle.

function(copy_resource_folder_next_to_target TARGET RESOURCE PATH_IN_BUNDLE)
  # On Windows/Linux simply copy the folder next to the executable
  set(TARGET_FOLDER_PATH "$<TARGET_FILE_DIR:${TARGET}>/${PATH_IN_BUNDLE}")
  # On OS X use the bundles resources directory 
  if(APPLE)
    set(TARGET_FOLDER_PATH "$<TARGET_FILE_DIR:${TARGET}>/../Resources/${PATH_IN_BUNDLE}")
  endif()

  # create Resource directory
  add_custom_command(TARGET ${TARGET} PRE_LINK COMMAND ${CMAKE_COMMAND} -E remove_directory ${TARGET_FOLDER_PATH})
  add_custom_command(TARGET ${TARGET} PRE_LINK COMMAND ${CMAKE_COMMAND} -E copy_directory ${RESOURCE}/ ${TARGET_FOLDER_PATH})
endfunction()

