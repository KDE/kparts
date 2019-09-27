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
ecm_generate_headers(KParts_CamelCase_HEADERS
    HEADER_NAMES
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
install(FILES ${KParts_CamelCase_HEADERS}
        DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KParts/KParts
        COMPONENT Devel)

add_library(KF5Parts ${KParts_LIB_SRCS})
add_library(KF5::Parts ALIAS KF5Parts)
ecm_generate_export_header(KF5Parts
    EXPORT_FILE_NAME ${KParts_BINARY_DIR}/kparts/kparts_export.h
    BASE_NAME KParts
    # GROUP_BASE_NAME KF <- enable once all of KF modules use ecm_generate_export_header
    VERSION ${KF5_VERSION}
    DEPRECATED_BASE_VERSION 0
    DEPRECATION_VERSIONS 5.0
    EXCLUDE_DEPRECATED_BEFORE_AND_AT ${EXCLUDE_DEPRECATED_BEFORE_AND_AT}
)

set(KParts_BUILD_INCLUDE_DIRS ${KParts_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(KF5Parts PUBLIC "$<BUILD_INTERFACE:${KParts_BUILD_INCLUDE_DIRS}>")
target_include_directories(KF5Parts INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KParts>" )

target_link_libraries(KF5Parts PUBLIC  KF5::KIOWidgets #browserrun.h uses krun.h
                                       KF5::XmlGui # essential to the technology
                                       KF5::TextWidgets # needed for KFind, as interface
                               PRIVATE KF5::I18n #few uses of i18n and i18nc, can be probably stripped down
                                       KF5::IconThemes #only used by KPart::iconLoader()
                      )

set_target_properties(KF5Parts PROPERTIES VERSION   ${KPARTS_VERSION_STRING}
                                          SOVERSION ${KPARTS_SOVERSION}
                                          EXPORT_NAME Parts
)

install(TARGETS KF5Parts EXPORT KF5PartsTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
   kpart.desktop
   krop.desktop
   krwp.desktop
   browserview.desktop
   DESTINATION ${KDE_INSTALL_KSERVICETYPES5DIR}
)
install(FILES
  ${KParts_BINARY_DIR}/kparts/kparts_export.h
  ${KParts_HEADERS}
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KParts/kparts COMPONENT Devel
)

install(FILES kde_terminal_interface.h DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KParts )


if(BUILD_QCH)
    ecm_add_qch(
        KF5Parts_QCH
        NAME KParts
        BASE_NAME KF5Parts
        VERSION ${KF5_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${KParts_HEADERS}
            kde_terminal_interface.h
        MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        LINK_QCHS
            KF5KIO_QCH
            KF5XmlGui_QCH
            KF5TextWidgets_QCH
        INCLUDE_DIRS
            ${KParts_BUILD_INCLUDE_DIRS}
        BLANK_MACROS
            KPARTS_EXPORT
            KPARTS_DEPRECATED
            KPARTS_DEPRECATED_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME KParts LIB_NAME KF5Parts DEPS "KIOWidgets KXmlGui KTextWidgets" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/KParts)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
