
set(UTILS_DIR ${CMAKE_CURRENT_LIST_DIR})

function(copy_assets_dir TARGET_NAME)
    add_custom_command(
    TARGET ${TARGET_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${UTILS_DIR}/assets"
    "$<TARGET_FILE_DIR:${TARGET_NAME}>/assets"
    COMMENT "Copying: \"${UTILS_DIR}/assets\" --> \"$<TARGET_FILE_DIR:${TARGET_NAME}>/assets\" ")
endfunction()
