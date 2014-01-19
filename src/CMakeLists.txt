set(KParts_LIB_SRCS
    partbase.cpp
    part.cpp
    openurlarguments.cpp
    readonlypart.cpp
    readwritepart.cpp
    plugin.cpp
    partmanager.cpp
    mainwindow.cpp
    event.cpp
    guiactivateevent.cpp
    partactivateevent.cpp
    partselectevent.cpp
    browserextension.cpp
    browserhostextension.cpp
    browserarguments.cpp
    liveconnectextension.cpp
    openurlevent.cpp
    windowargs.cpp
    historyprovider.cpp
    browserinterface.cpp
    browserrun.cpp
    browseropenorsavequestion.cpp
    statusbarextension.cpp
    scriptableextension.cpp
    textextension.cpp
    htmlextension.cpp
    htmlsettingsinterface.cpp
    selectorinterface.cpp
    fileinfoextension.cpp
    listingfilterextension.cpp
    listingnotificationextension.cpp
)

include(ECMGenerateHeaders)
ecm_generate_headers(
    BrowserArguments
    BrowserExtension
    BrowserHostExtension
    BrowserInterface
    BrowserOpenOrSaveQuestion
    BrowserRun
    Event
    FileInfoExtension
    GUIActivateEvent
    HistoryProvider
    HtmlExtension
    HtmlSettingsInterface
    ListingFilterExtension
    ListingNotificationExtension
    LiveConnectExtension
    MainWindow
    OpenUrlArguments
    OpenUrlEvent
    Part
    PartActivateEvent
    PartBase
    PartManager
    PartSelectEvent
    Plugin
    ReadOnlyPart
    ReadWritePart
    ScriptableExtension
    SelectorInterface
    StatusBarExtension
    TextExtension
    WindowArgs

    REQUIRED_HEADERS KParts_HEADERS
    PREFIX KParts
)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/KParts DESTINATION  ${INCLUDE_INSTALL_DIR} COMPONENT Devel )

add_library(KF5Parts ${KParts_LIB_SRCS})
generate_export_header(KF5Parts EXPORT_FILE_NAME ${KParts_BINARY_DIR}/kparts/kparts_export.h BASE_NAME KParts)
add_library(KF5::Parts ALIAS KF5Parts)

target_include_directories(KF5Parts PUBLIC "$<BUILD_INTERFACE:${KParts_BINARY_DIR}>")
target_include_directories(KF5Parts INTERFACE "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}/KParts>" )

target_link_libraries(KF5Parts PUBLIC  KF5::KIOWidgets #browserrun.h uses krun.h
                                       KF5::XmlGui # essential to the technology
                                       KF5::TextWidgets # needed for KFind, as interface
                               PRIVATE KF5::I18n #few uses of i18n and i18nc, can be probably stripped down
                                       KF5::Notifications #needed by browseropenorsavequestion.cpp
                                       KF5::IconThemes #only used by KPart::iconLoader()
                      )

if(HAVE_NEPOMUK)
    target_link_libraries(KF5Parts PRIVATE nepomuk nepomukutils)
endif()

set_target_properties(KF5Parts PROPERTIES VERSION   ${KPARTS_VERSION_STRING}
                                          SOVERSION ${KPARTS_SOVERSION}
                                          EXPORT_NAME Parts
)

install(TARGETS KF5Parts EXPORT KF5PartsTargets ${INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
   kpart.desktop
   krop.desktop
   krwp.desktop
   browserview.desktop
   DESTINATION ${SERVICETYPES_INSTALL_DIR}
)
install(FILES
  ${KParts_BINARY_DIR}/kparts/kparts_export.h
  ${KParts_HEADERS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/KParts/kparts COMPONENT Devel
)

install(FILES kde_terminal_interface.h DESTINATION ${INCLUDE_INSTALL_DIR}/KParts )

