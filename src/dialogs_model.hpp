#ifndef DIALOGS_MODEL_H
#define DIALOGS_MODEL_H

// QT
#include <QObject>
#include <QDebug>
#include <QAbstractListModel>

// CPP/STL
#include <cstdio>
#include <iostream>

// ANOTHERS
#include "src/instancemessenger.h"

class DialogModel : public QAbstractListModel {
    Q_OBJECT
    Q_ENUMS(DialogsRoles)

public:
    enum DialogRoles {
        Message = Qt::UserRole + 1,
        UserId
    };

//    using QAbstractListModel::QAbstractListModel;

    QHash<int,QByteArray> roleNames() const override {
        return { { Message, "message" },
                 { UserId, "userId" }
        };
    }

    void setDialog(const QList<DialogItem> &_dialog) {
        beginResetModel();
        m_dialog.clear();
        endResetModel();


        for (int i = 0; i < _dialog.size(); ++i) {
            beginInsertRows(QModelIndex(), m_dialog.size(), m_dialog.size());
            m_dialog.append(_dialog[i]);
            endInsertRows();
        }
    }

    void addDialogItem(const DialogItem &_userItem) {
        if (!m_dialog.contains(_userItem)) {
            beginInsertRows(QModelIndex(), m_dialog.size(), m_dialog.size());
            m_dialog.append(_userItem);
            endInsertRows();

            QModelIndex index = createIndex(0, 0);
            emit dataChanged(index, index);
        }
    }

    int rowCount(const QModelIndex & parent = QModelIndex()) const override {
        if (parent.isValid())
            return 0;
        return m_dialog.size();
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role) override {
        if (!hasIndex(index.row(), index.column(), index.parent()) || !value.isValid())
            return false;

        DialogItem &item = m_dialog[index.row()];
        switch(role) {
        case Message:
            item.m_message = value.toString();
            break;
        case UserId:
            item.m_user_id = value.toInt();
            break;
        default:
            return false;
        }

        emit dataChanged(index, index, {role});
        return true;
    }

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override {
        if (!hasIndex(index.row(), index.column(), index.parent()))
            return {};

        const DialogItem &item = m_dialog.at(index.row());
        switch(role) {
        case Message:
            return item.m_message;
        case UserId:
            return item.m_user_id;
        default:
            return {};
        }
    }

private:
    QList<DialogItem> m_dialog;
};

#endif // DIALOGS_MODEL_H
