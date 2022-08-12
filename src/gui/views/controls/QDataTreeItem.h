
#pragma once

#include <QTreeWidgetItem>
#include <memory>
#include <qobjectdefs.h>
#include <qtreewidget.h>


template <class T>
class QDataTreeItem : public QTreeWidgetItem {
public:
    QDataTreeItem(){};
    QDataTreeItem(QTreeWidgetItem* parent) : QTreeWidgetItem(parent){};
    ~QDataTreeItem(){};

    void setData(std::shared_ptr<T> data){
        this->data = data;
    };
    std::shared_ptr<T> getData(){
        return this->data;
    };
private:
     std::shared_ptr<T> data;
};
