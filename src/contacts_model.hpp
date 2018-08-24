#ifndef CONTACTS_MODEL_HPP
#define CONTACTS_MODEL_HPP

// QT
#include <QObject>
#include <QDebug>
#include <QAbstractListModel>

// CPP/STL
#include <cstdio>
#include <iostream>

// ANOTHERS
#include "src/instancemessenger.h"

class ContactsModel : public QAbstractListModel {
    Q_OBJECT
    Q_ENUMS(ContactsRoles)

public:
    enum ContactsRoles {
        Phone = Qt::UserRole + 1,
        UserId,
        Online
    };

//    using QAbstractListModel::QAbstractListModel;

    QHash<int,QByteArray> roleNames() const override {
        return { { Phone, "phone" },
                 { UserId, "userId" },
                 { Online, "online" }
        };
    }

    void setContacts(const QMap<int, UserItem> &_dialog) {
        beginResetModel();
        m_contacts.clear();
        endResetModel();

        QList<int> keys = _dialog.keys();

        for (int i = 0; i < keys.size(); ++i) {
            beginInsertRows(QModelIndex(), m_contacts.size(), m_contacts.size());
            m_contacts.append(_dialog[keys[i]]);
            endInsertRows();
        }
    }

    void addContact(const UserItem &_userItem) {
        if (!m_contacts.contains(_userItem)) {
            beginInsertRows(QModelIndex(), m_contacts.size(), m_contacts.size());
            m_contacts.append(_userItem);
            endInsertRows();

            QModelIndex index = createIndex(0, 0);
            emit dataChanged(index, index);
        }
    }

    int rowCount(const QModelIndex & parent = QModelIndex()) const override {
        if (parent.isValid())
            return 0;
        return m_contacts.size();
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role) override {
        if (!hasIndex(index.row(), index.column(), index.parent()) || !value.isValid())
            return false;

        UserItem &item = m_contacts[index.row()];
        switch(role) {
        case Phone:
            item.m_phone = value.toString();
            break;
        case UserId:
            item.m_id = value.toInt();
            break;
        case Online:
            item.m_isOnline = value.toBool();
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

        const UserItem &item = m_contacts.at(index.row());
        switch(role) {
        case Phone:
            return item.m_phone;
        case UserId:
            return item.m_id;
        case Online:
            return item.m_isOnline;
        default:
            return {};
        }
    }

private:
    QList<UserItem> m_contacts;
};

#endif // CONTACTS_MODEL_H
