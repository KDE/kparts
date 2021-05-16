add_definitions(-DTRANSLATION_DOMAIN=\"%{APPNAMELC}part\")

set(%{APPNAMELC}_PART_SRCS
   %{APPNAMELC}part.cpp
)

qt_add_resources(%{APPNAMELC}_PART_SRCS
    %{APPNAMELC}part.qrc
)

add_library(%{APPNAMELC}part MODULE ${%{APPNAMELC}_PART_SRCS})
kcoreaddons_desktop_to_json(%{APPNAMELC}part %{APPNAMELC}part.desktop)

target_link_libraries(%{APPNAMELC}part
    KF5::I18n
    KF5::Parts
)

install(TARGETS %{APPNAMELC}part  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf5/parts)
install(FILES %{APPNAMELC}part.desktop  DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
