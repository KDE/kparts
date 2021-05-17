add_executable(%{APPNAMELC})

target_sources(%{APPNAMELC} PRIVATE
    main.cpp
    %{APPNAMELC}shell.cpp
    # handled by CMake's AUTORCC, as set by KDECMakeSettings
    %{APPNAMELC}.qrc
)

target_link_libraries(%{APPNAMELC}
    KF5::I18n
    KF5::Parts
)

install(TARGETS %{APPNAMELC} ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS %{APPNAMELC}.desktop  DESTINATION  ${KDE_INSTALL_APPDIR})
