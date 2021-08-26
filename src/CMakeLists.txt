add_library(KF5Parts)
add_library(KF5::Parts ALIAS KF5Parts)

target_sources(KF5Parts PRIVATE
    partbase.cpp
    part.cpp
    partmetadatautil_p.cpp
    partloader.cpp
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
        PartLoader
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

ecm_qt_declare_logging_category(KF5Parts
    HEADER kparts_logging.h
    IDENTIFIER KPARTSLOG
    CATEGORY_NAME kf.parts
    DESCRIPTION "KParts"
    EXPORT KPARTSLOG
)

ecm_generate_export_header(KF5Parts
    EXPORT_FILE_NAME ${KParts_BINARY_DIR}/kparts/kparts_export.h
    BASE_NAME KParts
    GROUP_BASE_NAME KF
    VERSION ${KF_VERSION}
    DEPRECATED_BASE_VERSION 0
    DEPRECATION_VERSIONS 3.0 4.4 5.0 5.72 5.77 5.78 5.80 5.81 5.82 5.83
    EXCLUDE_DEPRECATED_BEFORE_AND_AT ${EXCLUDE_DEPRECATED_BEFORE_AND_AT}
)

target_include_directories(KF5Parts
    PUBLIC "$<BUILD_INTERFACE:${KParts_BINARY_DIR}>"
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KParts>"
)

target_link_libraries(KF5Parts PUBLIC  KF5::KIOWidgets #browserrun.h uses krun.h
                                       KF5::XmlGui # essential to the technology
                                       KF5::TextWidgets # needed for KFind, as interface
                               PRIVATE KF5::I18n #few uses of i18n and i18nc, can be probably stripped down
                                       KF5::IconThemes #only used by KPart::iconLoader()
                      )

set_target_properties(KF5Parts PROPERTIES VERSION   ${KPARTS_VERSION}
                                          SOVERSION ${KPARTS_SOVERSION}
                                          EXPORT_NAME Parts
)

install(TARGETS KF5Parts EXPORT KF5PartsTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
   kpart.desktop
   kparts-readonlypart.desktop
   kparts-readwritepart.desktop
   # KF6 TODO; rename to browser-view.desktop to match pattern as expected by desktop2json
   browserview.desktop
   DESTINATION ${KDE_INSTALL_KSERVICETYPES5DIR}
)
install(FILES
  ${KParts_BINARY_DIR}/kparts/kparts_export.h
  ${KParts_HEADERS}
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KParts/kparts COMPONENT Devel
)

install(FILES kde_terminal_interface.h DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KParts )

ecm_qt_install_logging_categories(
    EXPORT KPARTSLOG
    FILE kparts.categories
    DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
)

if(BUILD_QCH)
    ecm_add_qch(
        KF5Parts_QCH
        NAME KParts
        BASE_NAME KF5Parts
        VERSION ${KF_VERSION}
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
            ${CMAKE_BINARY_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
            KPARTS_EXPORT
            KPARTS_DEPRECATED
            KPARTS_DEPRECATED_EXPORT
            "KPARTS_DEPRECATED_VERSION(x, y, t)"
            "KPARTS_DEPRECATED_VERSION_BELATED(x, y, xt, yt, t)"
            "KPARTS_DEPRECATED_VERSION(x, y, t)"
            "KPARTS_ENUMERATOR_DEPRECATED_VERSION_BELATED(x, y, xt, yt, t)"
        PREDEFINED_MACROS
            "KIOWIDGETS_ENABLE_DEPRECATED_SINCE(x, y)=1"
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME KParts LIB_NAME KF5Parts DEPS "KIOWidgets KXmlGui KTextWidgets" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/KParts)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
