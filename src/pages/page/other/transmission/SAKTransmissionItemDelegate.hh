﻿/*
 * Copyright (C) 2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKTRANSMISSIONITEMDELEGATE_HH
#define SAKTRANSMISSIONITEMDELEGATE_HH

#include <QListWidget>
#include <QAbstractItemDelegate>

class SAKDebugPage;

class SAKTransmissionItemDelegate:public QAbstractItemDelegate
{
    Q_OBJECT
public:
    SAKTransmissionItemDelegate(SAKDebugPage *page, QObject *parent = Q_NULLPTR);

    enum TransmissionItemType {
        SerialPortType,
        UdpType,
        TcpType
    };
private:
    SAKDebugPage *debugPage;
    TransmissionItemType transmissionItemType = SerialPortType;
};

#endif
