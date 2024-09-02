QT       += core gui sql network printsupport serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
RC_FILE = exe.rc

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CustomClass/flatui.cpp \
    CustomClass/gaugepanel.cpp \
    CustomClass/lednumber.cpp \
    CustomClass/lightbutton.cpp \
    CustomClass/navbuttongroup/navbuttongroup.cpp \
    CustomClass/navlabel.cpp \
    CustomClass/progresscolor.cpp \
    CustomClass/slidertip.cpp \
    CustomClass/switchbutton.cpp \
    CustomClass/tumbler.cpp \
    CustomClass/tumblerdatetime.cpp \
    custom/customw.cpp \
    custom/databaseoperations.cpp \
    custom/mybtn.cpp \
    function_page/functiontest.cpp \
    loginwidget.cpp \
    main.cpp \
    maininterface.cpp \
    widget.cpp \
    function_page/conclusionshow.cpp \
    function_page/testinformation.cpp \
    custom/report.cpp \
    DataManagement/datamanagement.cpp \
    DataManagement/projectselect.cpp \
    DataManagement/databasemanag.cpp \
    user/userline.cpp \
    user/usermanagement.cpp \
    custom/popupwidget.cpp \
    log/loggg.cpp

HEADERS += \
    CustomClass/flatui.h \
    CustomClass/gaugepanel.h \
    CustomClass/lednumber.h \
    CustomClass/lightbutton.h \
    CustomClass/navbuttongroup/navbuttongroup.h \
    CustomClass/navlabel.h \
    CustomClass/progresscolor.h \
    CustomClass/slidertip.h \
    CustomClass/switchbutton.h \
    CustomClass/tumbler.h \
    CustomClass/tumblerdatetime.h \
    custom/customw.h \
    custom/databaseoperations.h \
    custom/mybtn.h \
    function_page/functiontest.h \
    globalVariable.h \
    loginwidget.h \
    maininterface.h \
    widget.h \
    function_page/conclusionshow.h \
    function_page/testinformation.h \
    custom/report.h \
    DataManagement/datamanagement.h \
    DataManagement/projectselect.h \
    DataManagement/databasemanag.h \
    user/userline.h \
    user/usermanagement.h \
    custom/popupwidget.h \
    log/loggg.h

FORMS += \
    function_page/functiontest.ui \
    loginwidget.ui \
    maininterface.ui \
    widget.ui \
    function_page/conclusionshow.ui \
    function_page/testinformation.ui \
    DataManagement/datamanagement.ui \
    DataManagement/projectselect.ui \
    DataManagement/databasemanag.ui \
    user/userline.ui \
    user/usermanagement.ui \
    log/loggg.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target






include(./CommunicationThread/CommunicationThread.pri)
include (./qtxlsx/src/xlsx/qtxlsx.pri)

RESOURCES += \
    res.qrc
