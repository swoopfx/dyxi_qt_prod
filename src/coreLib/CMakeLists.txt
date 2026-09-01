 
set(CMAKE_AUTOMOC ON)

# set(GENERAL_ROOT  ${CMAKE_CURRENT_SOURCE_DIR}/General)

# add_library(general_dir INTERFACE)

# target_include_directories(general_dir PUBLIC
#     ${GENERAL_ROOT}

# )
add_subdirectory(General)
add_subdirectory(Dataset)


add_library(coreLib)
target_link_libraries(coreLib PRIVATE General)



