# cmake_minimum_required(VERSION 3.5)

# project(untitled1 VERSION 0.1 LANGUAGES CXX)

# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
# find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# find_package(Qt6 COMPONENTS Core Sql Widgets REQUIRED)
# find_package(Qt6 REQUIRED COMPONENTS Core Sql Widgets Multimedia)


# file(GLOB_RECURSE PROJECT_SOURCES
#     "*.cpp"
#     "*.h"
#     "*.ui"
#     "history.h"
#     "history.cpp"
#     "history.ui"
# )

# if(QT_VERSION_MAJOR LESS 6)
#     if(ANDROID)
#         add_library(untitled1 SHARED
#             ${PROJECT_SOURCES}
#         )
#     else()
#         add_executable(untitled1
#             ${PROJECT_SOURCES}
#         )
#     endif()
# else()
#     qt_add_executable(untitled1
#         MANUAL_FINALIZATION
#         ${PROJECT_SOURCES}
#     )
# endif()


#  set(SQLITE_LIB_DIR " d:/Qt/6.6.1/mingw_64/lib")  # Замените на фактический путь
#  link_directories(${SQLITE_LIB_DIR})


# #target_link_libraries(untitled1 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Core Qt6::Sql Qt6::Widgets Qt6::Multimedia sqlite3)

# #target_link_libraries(untitled1 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Core Qt6::Sql Qt6::Widgets Qt6::Multimedia ${SQLITE_LIB_DIR}/sqlite3.dll)

# target_link_libraries(untitled1 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# target_link_libraries(untitled1 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Core Qt6::Sql Qt6::Widgets Qt6::Multimedia sqlite3)







# set_target_properties(untitled1 PROPERTIES
#     MACOSX_BUNDLE TRUE
#     WIN32_EXECUTABLE TRUE
#     ${BUNDLE_ID_OPTION}
#     MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#     MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
# )

# include(GNUInstallDirs)

# install(TARGETS untitled1
#     BUNDLE DESTINATION .
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )

# if(QT_VERSION_MAJOR EQUAL 6)
#     qt_finalize_executable(untitled1)
# endif()

cmake_minimum_required(VERSION 3.5)

project(untitled1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)





find_package(Qt6 COMPONENTS Core Sql Widgets REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Sql Widgets Multimedia)





#find_package(Sqlite3 REQUIRED)


file(GLOB_RECURSE PROJECT_SOURCES
    "*.cpp"
    "*.h"
    "*.ui"
    "history.h"
    "history.cpp"
    "history.ui"
)


# set(PROJECT_SOURCES
#         main.cpp
#         mainwindow.cpp
#         mainwindow.h
#         mainwindow.ui
# )

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(untitled1
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET untitled4 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(untitled1 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(untitled1
            ${PROJECT_SOURCES}
        )
    endif()
endif()

#target_link_libraries(untitled4 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Core Qt6::Sql Qt6::Widgets Qt6::Multimedia sqlite3)

target_link_libraries(untitled1 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Core Qt6::Sql Qt6::Widgets Qt6::Multimedia ${SQLite3_LIBRARIES})


target_link_libraries(untitled1 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)


#target_link_libraries(untitled4 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Core Qt6::Sql Qt6::Widgets Qt6::Multimedia d:\Qt\6.6.1\mingw_64\plugins\sqldrivers\sqlite.dll)



#target_link_libraries(untitled4 PRIVATE Qt6::Core Qt6::Sql Qt6::Widgets Qt6::Multimedia sqlite)











# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.untitled4)
endif()
set_target_properties(untitled1 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS untitled1
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(untitled1)
endif()
