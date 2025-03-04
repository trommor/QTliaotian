#include "addfriend.h"
#include "ui_addfriend.h"
#include <QDebug>
addfriend::addfriend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addfriend)
{
    ui->setupUi(this);
}

addfriend::~addfriend()
{

    delete ui;
}
