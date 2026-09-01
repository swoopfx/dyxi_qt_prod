set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
add_library(Dataset STATIC

) 

qt_add_qml_module(Dataset
  URI Dataset
  SOURCES tracinglevel1dataset.h tracinglevel1dataset.cpp
  SOURCES recognitionshapeexplorerdataset.h recognitionshapeexplorerdataset.cpp
)

target_link_libraries(Dataset
  PRIVATE
  Qt6::Core
  Qt6::Qml
  Qt6::Network
)
