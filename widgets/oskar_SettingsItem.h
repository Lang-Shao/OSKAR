/*
 * Copyright (c) 2012, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef OSKAR_SETTINGS_ITEM_H_
#define OSKAR_SETTINGS_ITEM_H_

/**
 * @file oskar_SettingsItem.h
 */

#include <QtCore/QList>
#include <QtCore/QVector>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

class oskar_SettingsItem
{
public:
    enum {
        LABEL,              /* e.g. "Sky model settings". */
        BOOL,               /* e.g. disable station beam. */
        INT,
        INT_UNSIGNED,
        INT_POSITIVE,
        DOUBLE,
        DOUBLE_MAX,
        DOUBLE_MIN,
        STRING,
        TELESCOPE_DIR_NAME,
        OUTPUT_FILE_NAME,   /* e.g. visibility data. */
        INPUT_FILE_NAME,    /* e.g. sky model file. */
        INPUT_FILE_LIST,    /* e.g. MeerKAT antenna files. */
        INT_CSV_LIST,       /* e.g. CUDA device IDs. */
        OPTIONS,            /* e.g. noise type. */
        RANDOM_SEED,
        AXIS_RANGE,
        DATE_TIME,          /* e.g. observation start time. */
        TIME                /* e.g. observation duration. */
    };

public:
    oskar_SettingsItem(const QString& key, const QString& subkey, int type,
            const QString& label, const QVariant& value, bool required = false,
            const QVariant& defaultValue = QVariant(),
            const QStringList& options = QStringList(),
            oskar_SettingsItem* parent = 0);
    ~oskar_SettingsItem();

    void appendChild(oskar_SettingsItem* child);
    oskar_SettingsItem* child(int row);
    int childCount() const;
    int childNumber() const;
    int critical() const;
    const QVariant& defaultValue() const;
    bool enabled() const;
    const QVariant& iterationInc() const;
    int iterationNum() const;
    const QString& key() const;
    const QString& label() const;
    const QStringList& options() const;
    oskar_SettingsItem* parent();
    bool required() const;
    void setDefaultValue(const QVariant& value);
    void setEnabled(bool value);
    void setIterationInc(const QVariant& value);
    void setIterationNum(int value);
    void setLabel(const QString& value);
    void setTooltip(const QString& value);
    void setValue(const QVariant& value);
    const QString& subkey() const;
    const QString& tooltip() const;
    int type() const;
    const QVariant& value() const;
    int valueSet() const;
    int visible() const;

private:
    void setCritical(bool value);
    void setRequired(bool value);
    void setValueSet(bool value);
    void setVisible(bool value);

private:
    oskar_SettingsItem* parentItem_;
    QList<oskar_SettingsItem*> childItems_;
    QString key_; // Full settings key for the item.
    QString subkey_; // Short settings key.
    int type_; // Enumerated type.
    int visible_;
    int valueSet_;
    bool enabled_;
    bool required_;
    QString label_;
    QString tooltip_;
    QVariant value_;
    QVariant defaultValue_;
    QStringList options_;
    int critical_;
    int iterNum_;
    QVariant iterInc_;
};

#endif /* OSKAR_SETTINGS_ITEM_H_ */
