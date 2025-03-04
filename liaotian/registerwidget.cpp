#include "registerwidget.h"
#include "ui_registerwidget.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QSqlQuery>
#include <QSqlError>
RegisterWidget::RegisterWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegisterWidget)
{
    ui->setupUi(this);

    // 连接注册按钮的点击信号到槽函数
    connect(ui->registerButton, &QPushButton::clicked, this, &RegisterWidget::on_registerButton_clicked);
}

RegisterWidget::~RegisterWidget()
{
    delete ui;
}

void RegisterWidget::on_registerButton_clicked()
{
    // 获取用户名、密码和确认密码
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    QString confirmPassword = ui->confirmPasswordLineEdit->text();

    // 验证输入是否为空
    if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "账号和密码不能为空！");
        return;
    }

    // 验证密码和确认密码是否一致
    if (password != confirmPassword) {
        QMessageBox::warning(this, "输入错误", "两次输入的密码不一致！");
        return;
    }

    // 插入数据到数据库
      QSqlQuery query;
      query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
      query.bindValue(":username", username);
      query.bindValue(":password", password);

      if (!query.exec()) {
          if (query.lastError().text().contains("Duplicate entry")) {
              QMessageBox::warning(this, "注册失败", "用户名已存在！");
          } else {
              QMessageBox::warning(this, "注册失败", "数据库错误：" + query.lastError().text());
          }
          return;
      }
    // 显示注册成功消息
    QMessageBox::information(this, "注册成功", "账号注册成功！");

    // 关闭注册页面
    this->close();
}
