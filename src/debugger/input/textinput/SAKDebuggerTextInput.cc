﻿/*
 * Copyright (C) 2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QDebug>

#include "SAKDebuggerTextInput.hh"
#include "SAKDebuggerInputSettings.hh"
#include "SAKDebuggerInputTextFactory.hh"

SAKDebuggerTextInput::SAKDebuggerTextInput(SAKDebugger *debugger, QObject *parent)
    :QObject(parent)
    ,debugger(debugger)
    ,_addCRCFlag(false)
    ,_bigEndianCRCFlag(false)
{
    inputTextFactory = new SAKDebuggerInputTextFactory;
    inputTextFactory->setTextInput(this);
    connect(inputTextFactory, &SAKDebuggerInputTextFactory::dataCooked, this, &SAKDebuggerTextInput::writeBytesRequest);
}

SAKDebuggerTextInput::~SAKDebuggerTextInput()
{
    inputTextFactory->requestInterruption();
    inputTextFactory->exit();
    inputTextFactory->wakeMe();
    inputTextFactory->wait();
    delete inputTextFactory;
}

void SAKDebuggerTextInput::writeBytes(QByteArray bytes)
{
    emit writeBytesRequest(bytes);
}

void SAKDebuggerTextInput::writeRawData(QString data)
{
    inputTextFactory->addRawData(data.length() ? data : QString("empty"));
}

bool SAKDebuggerTextInput::paraAddCRCFlag()
{
    addCRCFlagMutex.lock();
    bool flag = _addCRCFlag;
    addCRCFlagMutex.unlock();

    return flag;
}

bool SAKDebuggerTextInput::paraBigEndianCRCFlag()
{
    bigEndianCRCFlagMutex.lock();
    bool flag = _bigEndianCRCFlag;
    bigEndianCRCFlagMutex.unlock();

    return flag;
}

void SAKDebuggerTextInput::setInputSettings(SAKDebuggerInputSettings *settings)
{
    inputSettings = settings;
    inputTextFactory->setInputSettings(inputSettings);
}

QString SAKDebuggerTextInput::formattingInputText(QString srcString)
{
    QString formattedText;
    int format = inputSettings->paraCurrentInputFromat();

    if (!srcString.isEmpty()){
        if (format == SAKDebuggerInputSettings::Bin){
            QString strTemp;
            srcString.remove(QRegExp("[^0-1]"));
            for (int i = 0; i < srcString.length(); i++){
                if ((i != 0) && (i % 8 == 0)){
                    formattedText.append(QChar(' '));
                }
                formattedText.append(srcString.at(i));
            }
        }else if(format == SAKDebuggerInputSettings::Otc) {
            srcString.remove(QRegExp("[^0-7]"));
            for (int i = 0; i < srcString.length(); i++){
                if ((i != 0) && (i % 2 == 0)){
                    formattedText.append(QChar(' '));
                }
                formattedText.append(srcString.at(i));
            }
        }else if(format == SAKDebuggerInputSettings::Dec) {
            srcString.remove(QRegExp("[^0-9]"));
            for (int i = 0; i < srcString.length(); i++){
                if ((i != 0) && (i % 2 == 0)){
                    formattedText.append(QChar(' '));
                }
                formattedText.append(srcString.at(i));
            }
        }else if(format == SAKDebuggerInputSettings::Hex) {
            srcString.remove(QRegExp("[^0-9a-fA-F]"));
            for (int i = 0; i < srcString.length(); i++){
                if ((i != 0) && (i % 2 == 0)){
                    formattedText.append(QChar(' '));
                }
                formattedText.append(srcString.at(i));
            }
        }else if(format == SAKDebuggerInputSettings::Ascii) {
            srcString.remove(QRegExp("[^\0u00-\u007f ]"));
        }else if(format == SAKDebuggerInputSettings::Utf8) {
            /// @brief nothing to do
        }else if(format == SAKDebuggerInputSettings::System) {
            /// @brief nothing to do
        }else {
            Q_ASSERT_X(false, __FUNCTION__, "Unknow input model");
        }
    }

    return formattedText;
}

bool SAKDebuggerTextInput::addCRCFlag()
{
    addCRCFlagMutex.lock();
    bool flag = _addCRCFlag;
    addCRCFlagMutex.unlock();

    return flag;
}

void SAKDebuggerTextInput::setAddCRCFlag(bool flag)
{
    addCRCFlagMutex.lock();
    _addCRCFlag = flag;
    addCRCFlagMutex.unlock();

    emit addCRCFlagChanged();
}

bool SAKDebuggerTextInput::bigEndianCRCFlag()
{
    bigEndianCRCFlagMutex.lock();
    bool flag = _bigEndianCRCFlag;
    bigEndianCRCFlagMutex.unlock();

    return flag;
}

void SAKDebuggerTextInput::setBigEndianCRCFlag(bool flag)
{
    bigEndianCRCFlagMutex.lock();
    _bigEndianCRCFlag = flag;
    bigEndianCRCFlagMutex.unlock();

    emit bigEndianCRCFlagChanged();
}