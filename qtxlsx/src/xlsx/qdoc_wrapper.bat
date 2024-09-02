@echo off
SetLocal EnableDelayedExpansion
(set QT_VERSION=0.3.0)
(set QT_VER=0.3)
(set QT_VERSION_TAG=030)
(set QT_INSTALL_DOCS=D:/Qt5.12.0/Docs/Qt-5.12.0)
(set BUILDDIR=D:/Qt5.7.0/QtXlsxWriter-master/src/xlsx)
D:\Qt5.12.0\5.12.0\mingw73_64\bin\qdoc.exe %*
EndLocal
