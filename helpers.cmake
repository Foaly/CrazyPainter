function(copy_resource_folder_to_bundle TARGET RESOURCE PATH_IN_BUNDLE)
  if(NOT APPLE)
    message( WARNING "copy_resource_folder_to_bundle called on non-apple platform" )
    return()
  endif()

  # create Resource directory
  add_custom_command(TARGET ${TARGET} PRE_LINK COMMAND ${CMAKE_COMMAND} -E remove_directory "$<TARGET_FILE_DIR:${TARGET}>/../Resources/${PATH_IN_BUNDLE}")
  add_custom_command(TARGET ${TARGET} PRE_LINK COMMAND ${CMAKE_COMMAND} -E copy_directory ${RESOURCE}/ "$<TARGET_FILE_DIR:${TARGET}>/../Resources/${PATH_IN_BUNDLE}")
endfunction()
