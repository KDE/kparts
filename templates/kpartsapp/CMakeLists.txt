cmake_minimum_required(VERSION 3.16)

project(%{APPNAMELC})

set(REQUIRED_ECM_VERSION "5.83.0")
find_package(ECM ${REQUIRED_ECM_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMInstallIcons)
include(FeatureSummary)

set(QT_MIN_VERSION "5.15.0")
find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Widgets
)

set(REQUIRED_KF_VERSION "5.23.0")
find_package(KF5 ${REQUIRED_KF_VERSION} REQUIRED COMPONENTS
    I18n
    Parts
)

add_subdirectory(src)
add_subdirectory(icons)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
