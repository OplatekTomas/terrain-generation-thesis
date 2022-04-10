//
// Created by tomas on 08.04.22.
//

#include <ui/BaseWidget.h>
#include <iostream>
#include <QEvent>
#include <QWidget>
#include <QCoreApplication>

namespace MapGenerator {
    BaseWidget::BaseWidget(QWidget *parent) : QWidget(parent) {
        setFocusPolicy(Qt::StrongFocus);

        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        this->currentLayout = new QVBoxLayout();
        this->setLayout(currentLayout);
        btn = new QPushButton("Generate");
        currentLayout->addWidget(btn);
        connect(btn, &QPushButton::released, this, [this]() {
            std::cout << "Tvoje máma" << std::endl;
        });
        this->label = new QLabel("Test label", this);
        this->label->setTextFormat(Qt::RichText);
        //set font color as blue
        QString styleSheet = "QLabel { color : blue; }";
        this->label->setStyleSheet(styleSheet);

        currentLayout->addWidget(label);
        btn->move(10, 10);

        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

    void BaseWidget::mouseMoveEvent(QMouseEvent *event) {
        QWidget::mouseMoveEvent(event);
    }

    bool BaseWidget::eventFilter(QObject *obj, QEvent *event) {
            if (event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            QPoint pos = mouseEvent->pos();
            auto test = childAt(pos);
            if (test != nullptr) {
                test->setFocus();
                QEvent* ev = new QEvent(*event);
                qApp->sendEvent(test, ev);
                std::cout << "Base got en event: " << event->type() << " for something with ptr:" << test << std::endl;
            }
        }
        if(event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            std::cout << "Key press: " << keyEvent->key() << std::endl;
        }
        return QWidget::eventFilter(obj, event);
        //return QObject::eventFilter(obj, event);
    }


}
