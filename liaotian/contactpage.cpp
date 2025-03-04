#include "contactpage.h"
#include <QListWidget>
#include <QVBoxLayout>

ContactPage::ContactPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 联系人列表
    contactList = new QListWidget(this);
    contactList->addItems({"用户1", "用户2", "用户3"});
    layout->addWidget(contactList);
}
