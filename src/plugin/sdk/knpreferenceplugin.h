/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNPREFERENCEPLUGIN_H
#define KNPREFERENCEPLUGIN_H

#include <QObject>

class KNPreferencePlugin : public QObject
{
    Q_OBJECT
public:
    KNPreferencePlugin(QObject *parent = 0):QObject(parent){}
    virtual QWidget *preferencePanel()=0;

signals:
    void requireHidePreference();

public slots:


};

#endif // KNPREFERENCEPLUGIN_H
