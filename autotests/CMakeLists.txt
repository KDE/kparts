find_package(Qt${QT_MAJOR_VERSION}Test ${REQUIRED_QT_VERSION} REQUIRED)
set_package_properties(Qt${QT_MAJOR_VERSION}Test PROPERTIES PURPOSE "Required for tests")

include(ECMAddTests)

########### a KParts ###############

# don't use kcoreaddons_add_plugin here since we don't want to install it
add_library(notepadpart MODULE)
target_sources(notepadpart PRIVATE notepad.cpp notepad.qrc)
# so we have to do the INSTALL_NAMESPACE thing by hand:
set_target_properties(notepadpart PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/kf5/parts")
kcoreaddons_desktop_to_json(notepadpart notepad.desktop)
target_link_libraries(notepadpart KF5::Parts)

########### tests ###############

ecm_add_tests(
  parttest.cpp
  partloadertest.cpp
  openorsavequestion_unittest.cpp
  LINK_LIBRARIES KF5::Parts Qt${QT_MAJOR_VERSION}::Test KF5::XmlGui
)
