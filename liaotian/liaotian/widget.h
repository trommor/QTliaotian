#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "registerwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_logobtBt_clicked();
    void on_gotoRegisterButton_clicked();

private:
    Ui::Widget *ui;
    RegisterWidget *registerWidget = nullptr; // 保存注册页面实例
    bool initializeDatabase();
};

#endif // WIDGET_H
