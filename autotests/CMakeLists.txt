find_package(Qt5Test ${REQUIRED_QT_VERSION} REQUIRED)
set_package_properties(Qt5Test PROPERTIES PURPOSE "Required for tests")

include(ECMAddTests)

ecm_add_tests(
  parttest.cpp
  openorsavequestion_unittest.cpp
  NAME_PREFIX "kparts-"
  LINK_LIBRARIES KF5::Parts Qt5::Test KF5::XmlGui
)
