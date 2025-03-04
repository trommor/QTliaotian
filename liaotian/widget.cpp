#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <mainwindow.h>
#include <QIntValidator>
#include "registerwidget.h"
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 设置账号输入框只允许输入数字
    QIntValidator *validator = new QIntValidator(this);
    ui->usernameLineEdit->setValidator(validator);

    ui->statusLabel->setText("请输入用户名和密码");

    // 初始化数据库
    if (!initializeDatabase()) {
        QMessageBox::critical(this, "错误", "无法连接数据库！");
        return;
    }

    // 连接登录按钮的点击信号到槽函数
    connect(ui->logobtBt, &QPushButton::clicked, this, &Widget::on_logobtBt_clicked);

    // 连接跳转到注册页面的按钮的点击信号到槽函数
    connect(ui->gotoRegisterButton, &QPushButton::clicked, this, &Widget::on_gotoRegisterButton_clicked);

    // 连接回车键信号到登录槽函数
    connect(ui->passwordLineEdit, &QLineEdit::returnPressed, this, &Widget::on_logobtBt_clicked);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_logobtBt_clicked()
{
    // 获取用户名和密码
    QString account = ui->usernameLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text().trimmed();

    // 查询数据库
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE account = :account");
    query.bindValue(":account", account);

    if (!query.exec()) {
        QMessageBox::warning(this, "数据库错误", "查询失败：" + query.lastError().text());
        return;
    }

    // 验证账号和密码
    if (query.next()) {
        QString storedPassword = query.value(0).toString();
        if (storedPassword == password) {
            QMessageBox::information(this, "登录成功", "欢迎回来，" + account + "!");

            // 创建并显示 MainWindow
            MainWindow *mainWindow = new MainWindow();
            mainWindow->show();

            // 关闭当前登录页面
            this->close();
        } else {
            QMessageBox::warning(this, "登录失败", "密码错误");
        }
    } else {
        QMessageBox::warning(this, "登录失败", "用户名不存在");
    }
}

bool Widget::initializeDatabase()
{
    // 添加 MySQL 数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");      // 数据库主机名
    db.setPort(3306);                // 数据库端口
    db.setDatabaseName("my_database");   // 数据库名称
    db.setUserName("root");           // 数据库用户名
    db.setPassword("1804");            // 数据库密码

    // 打开数据库
    if (!db.open()) {
        qDebug() << "无法连接数据库：" << db.lastError().text();
        return false;
    }

    qDebug() << "数据库连接成功！";
    return true;
}

void Widget::on_gotoRegisterButton_clicked()
{
    // 防抖：如果注册页面已经显示，则不再创建新实例
    if (registerWidget && registerWidget->isVisible()) {
        return;
    }

    // 创建并显示注册页面
    registerWidget = new RegisterWidget();
    registerWidget->show();

    // 隐藏当前登录页面
    //this->hide();
}
