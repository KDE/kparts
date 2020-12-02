set(%{APPNAMELC}_SRCS
   main.cpp
   %{APPNAMELC}shell.cpp
)

qt5_add_resources(%{APPNAMELC}_SRCS
    %{APPNAMELC}.qrc
)

add_executable(%{APPNAMELC} ${%{APPNAMELC}_SRCS})

target_link_libraries(%{APPNAMELC}
    KF5::I18n
    KF5::Parts
)

install(TARGETS %{APPNAMELC} ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS %{APPNAMELC}.desktop  DESTINATION  ${KDE_INSTALL_APPDIR})
