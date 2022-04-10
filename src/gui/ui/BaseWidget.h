//
// Created by tomas on 08.04.22.
//


#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>


namespace MapGenerator{
    class BaseWidget : public QWidget {
        Q_OBJECT



    public:
        explicit BaseWidget(QWidget *parent = nullptr);
        ~BaseWidget() = default;
        QPushButton* btn;
        QVBoxLayout *currentLayout;

        //The event filter
        bool eventFilter(QObject *obj, QEvent *event) override;

    protected slots:
        //mouse move event
        void mouseMoveEvent(QMouseEvent *event) override;

        //mouse press event
        //Generic event handler
    private:
        QLabel *label;
    };

}



