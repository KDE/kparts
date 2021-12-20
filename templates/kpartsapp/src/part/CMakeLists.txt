add_definitions(-DTRANSLATION_DOMAIN=\"%{APPNAMELC}part\")

add_library(%{APPNAMELC}part MODULE)

target_sources(%{APPNAMELC}part PRIVATE
    %{APPNAMELC}part.cpp
    # handled by CMake's AUTORCC, as set by KDECMakeSettings
    %{APPNAMELC}part.qrc
)

kcoreaddons_desktop_to_json(%{APPNAMELC}part %{APPNAMELC}part.desktop)

target_link_libraries(%{APPNAMELC}part
    KF5::I18n
    KF5::Parts
)

install(TARGETS %{APPNAMELC}part  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf5/parts)
install(FILES %{APPNAMELC}part.desktop  DESTINATION ${KDE_INSTALL_KSERVICESDIR})
