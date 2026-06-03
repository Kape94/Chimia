
set(UTILS_DIR ${CMAKE_CURRENT_LIST_DIR})

function(copy_assets_dir TARGET_NAME)
    add_custom_command(
    TARGET ${TARGET_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${UTILS_DIR}/assets"
    "$<TARGET_FILE_DIR:${TARGET_NAME}>/assets"
    COMMENT "Copying...\n-> source: \"${UTILS_DIR}/assets\"\n-> dest: \"$<TARGET_FILE_DIR:${TARGET_NAME}>/assets\" ")
endfunction()
