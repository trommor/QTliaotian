#ifndef CONTACTPAGE_H
#define CONTACTPAGE_H

#include <QWidget>
#include <QListWidget>
class ContactPage : public QWidget {
    Q_OBJECT

public:
    explicit ContactPage(QWidget *parent = nullptr);

private:
    QListWidget *contactList;
};

#endif // CONTACTPAGE_H
