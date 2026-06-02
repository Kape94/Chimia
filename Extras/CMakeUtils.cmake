
function(copy_assets_dir TARGET_NAME)
    add_custom_command(
    TARGET ${TARGET_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:${TARGET_NAME}>/assets"
    COMMENT "Copying: \"${CMAKE_CURRENT_SOURCE_DIR}/assets\" --> \"$<TARGET_FILE_DIR:${TARGET_NAME}>/assets\" ")
endfunction()
