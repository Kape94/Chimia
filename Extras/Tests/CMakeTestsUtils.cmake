
function(copy_golden_artifacts_dir TARGET_NAME)
    add_custom_command(
        TARGET ${TARGET_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/goldenArtifacts"
        "$<TARGET_FILE_DIR:${TARGET_NAME}>/goldenArtifacts"
        COMMENT "Copying...\n-> source: \"${CMAKE_CURRENT_SOURCE_DIR}/goldenArtifacts\"\n-> dest: \"$<TARGET_FILE_DIR:${TARGET_NAME}>/goldenArtifacts\" ")
endfunction()
