find_package(Qt${QT_MAJOR_VERSION}Test ${REQUIRED_QT_VERSION} REQUIRED)
set_package_properties(Qt${QT_MAJOR_VERSION}Test PROPERTIES PURPOSE "Required for tests")

include(ECMMarkAsTest)

add_library(parts_test_interface INTERFACE)
target_link_libraries(parts_test_interface INTERFACE
    Qt${QT_MAJOR_VERSION}::Test KF5::Parts KF5::CoreAddons KF5::I18n KF5::XmlGui KF5::KIOCore
)

add_executable(kpartstest testmainwindow.cpp parts.cpp)
ecm_mark_as_test(kpartstest)
target_link_libraries(kpartstest parts_test_interface)

add_executable(normalktmtest normalktm.cpp parts.cpp)
ecm_mark_as_test(normalktmtest)
target_link_libraries(normalktmtest parts_test_interface)


########### next target ###############

if(NOT EXCLUDE_DEPRECATED_BEFORE_AND_AT STREQUAL "CURRENT" AND
   EXCLUDE_DEPRECATED_BEFORE_AND_AT VERSION_LESS 5.90.0)
    add_library(spellcheckplugin MODULE plugin_spellcheck.cpp)
    target_link_libraries(spellcheckplugin parts_test_interface )
    install(TARGETS spellcheckplugin  DESTINATION ${KDE_INSTALL_PLUGINDIR} )
endif()

########### unit tests ###############

MACRO(KPARTS_EXECUTABLE_TESTS)
       FOREACH(_testname ${ARGN})
               add_executable(${_testname}  ${_testname}.cpp)
               ecm_mark_as_test(${_testname})
               target_link_libraries(${_testname} parts_test_interface)
       ENDFOREACH(_testname)
ENDMACRO(KPARTS_EXECUTABLE_TESTS)

KPARTS_EXECUTABLE_TESTS(
    partviewer
    openorsavequestion
    terminal_test
)

########### install files ###############

# Normally this would be needed. For the test programs we can avoid that, see addResourceDir.
#install( FILES notepadpart.rc  DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/notepadpart )
#install( FILES partviewer_shell.rc  DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/partviewer )
#install( FILES kpartstest_shell.rc  DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/kpartstest )
#install( FILES kpartstest_part1.rc  DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/kpartstestpart )

# To test plugins without installing them, try "ln -s kpartplugins <srcdir>"
#install( FILES plugin_foobar.rc plugin_spellcheck.rc  DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/notepadpart/kpartplugins )
