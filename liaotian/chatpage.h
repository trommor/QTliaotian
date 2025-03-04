#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
class ChatPage : public QWidget {
    Q_OBJECT

public:
    explicit ChatPage(QWidget *parent = nullptr);

private:
    QTextEdit *chatHistory;  // 聊天记录显示区域
    QLineEdit *messageInput; // 消息输入框
    QPushButton *sendButton; // 发送按钮
    void sendMessage();
    QLabel *messageLabel;
};

#endif // CHATPAGE_H
