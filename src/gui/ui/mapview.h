//
// Created by tomas on 12.04.22.
//

#ifndef VUT_BP_MAPVIEW_H
#define VUT_BP_MAPVIEW_H

#include <QWidget>
#include <ui/Map.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MapView; }
QT_END_NAMESPACE

class MapView : public QWidget {
Q_OBJECT

public:
    explicit MapView(QWidget *parent = nullptr);

    ~MapView() override;

protected slots:
    void onSearch();
    void onDefaultClicked();

private:
    Ui::MapView *ui;
};


#endif //VUT_BP_MAPVIEW_H
