#include "chatpage.h"
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateTime>  // 包含 QDateTime 头文件
#include <QString>
#include <QFont>
#include <QPalette>

ChatPage::ChatPage(QWidget *parent)
    : QWidget(parent)
{
    // 创建控件
    chatHistory = new QTextEdit(this);
    chatHistory->setReadOnly(true);  // 设置为只读

    messageInput = new QLineEdit(this);
    sendButton = new QPushButton("发送", this);

    // 设置聊天历史框的样式
    chatHistory->setStyleSheet("QTextEdit { font-size: 14px; color: black; background-color: white; }");

    // 设置字体
    QFont font;
    font.setPointSize(14);  // 设置字体大小
    chatHistory->setFont(font);

    // 设置调色板
    QPalette palette = chatHistory->palette();
    palette.setColor(QPalette::Text, Qt::black);  // 设置文本颜色
    palette.setColor(QPalette::Base, Qt::white);  // 设置背景颜色
    chatHistory->setPalette(palette);

    // 设置滚动条策略
    chatHistory->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);  // 始终显示垂直滚动条
    chatHistory->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 隐藏水平滚动条

    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chatHistory);

    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(messageInput);
    inputLayout->addWidget(sendButton);

    layout->addLayout(inputLayout);
    setLayout(layout);

    // 连接发送按钮的点击信号到槽函数
    connect(sendButton, &QPushButton::clicked, this, &ChatPage::sendMessage);

    // 连接回车键信号到槽函数
    connect(messageInput, &QLineEdit::returnPressed, this, &ChatPage::sendMessage);

    // 初始化聊天记录
    chatHistory->append("用户1: 你好！");
    chatHistory->append("<b>用户1:</b> <font color='blue'>你好！</font>");
}

// 发送消息的槽函数
void ChatPage::sendMessage()
{
    QString message = messageInput->text();
    if (!message.isEmpty()) {
        QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
        chatHistory->append("[" + currentTime + "] <b>我:</b> " + message);  // 将消息添加到聊天记录
        messageInput->clear();  // 清空输入框
    }
}
