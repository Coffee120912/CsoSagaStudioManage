#pragma once
#include <CACEInterface.h>
#include <QtWidgets/QApplication>
#include <QCryptographicHash>
#include <QDesktopServices>
#include <qdir.h>
#include <QSettings.h>
#include <QTextCodec.h>
#include <QMessageBox.h>
#include <QTimer>
#include <QCloseEvent>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QNetworkInterface>
#include <qlist.h>
#include <PackReader.h>
#include <PackWrite.h>
#include <QDateTime>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <QComboBox>
#include <QFileDialog>
#include <QMenu>
#include <QtNetwork\QNetworkAccessManager>
#include <QtNetwork\QNetworkReply>
#include <QtNetwork\QNetworkRequest>
#include <QDebug>
#include <QtWidgets/QWidget>
#include <QtWidgets/QFileDialog>

#include "../Connect/ShareDef.h"
#include "LoginModule.h"

#include <CACEInterface.h>
#include <thread>

#include <RCF/RCF.hpp>
#include <RCF/RcfServer.hpp>
#include <RCF/Win32NamedPipeEndpoint.hpp>
#include <RCF../../../RCFCsoStudioInterface.hpp>

#pragma comment(lib,"openssl/win32/lib/libcrypto.lib")
#pragma comment(lib,"openssl/win32/lib/libssl.lib")
#pragma comment(lib,"Crypt32.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"wldap32.lib")
#pragma comment(lib,"RcfDll.lib")





#pragma execution_character_set("utf-8")


#define DATETIME qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))


extern QSettings* g_pAppConfig;
