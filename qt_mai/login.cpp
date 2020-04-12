#include "login.h"
#include "ui_login.h"
#include <QDebug>
#include <QFile>

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    connect(ui->btnNewAccount, SIGNAL(clicked()),
            this, SLOT(addNewAccount()));
}

Login::~Login()
{
    delete ui;
}

void Login::on_btnLogin_clicked()
{
    login = ui->lineLogin->text();
    password = ui->linePassword->text();

    if(login.isEmpty() || password.isEmpty()){
        qDebug()<<"Err: Login or Password empty!";
        return;
    }

    QFile openedFile("Accounts.txt");
    if (!openedFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"File with this name is not found, please add new account";
        return;
    }
    QTextStream stream(&openedFile);
    QStringList tempList = stream.readAll().split("\n");

    foreach(QString i, tempList){
        if(i.split(";")[0].contains(login)){
            tempList = i.split(";");
            break;
        } else if(i == i.end()){
            qDebug()<<"This acount does't exist!";
            return;
        }
    }

    QString key = crypted.decript(tempList[1].split(":"));
    if(key == password){
        m = new MainWindow(login);
        m->show();
        this->close();
        connect(m, SIGNAL(logout()),
                this, SLOT(openWindow()));
    } else {
        qDebug()<<"Err: Login or Password is wrong!";
    }
}

void Login::on_btnExit_clicked()
{
    this->close();
}

void Login::openWindow()
{
    delete m;
    m = new MainWindow;
    connect(m, SIGNAL(logout()),
            this, SLOT(openWindow()));
    this->show();
}

void Login::addNewAccount()
{
    QFile openedFile("Accounts.txt");
    if (!openedFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        return;
    }
    QTextStream stream(&openedFile);
    QStringList tempList = stream.readAll().split("\n");

    login = ui->lineLogin->text();
    password = ui->linePassword->text();

    if(login.isEmpty() || password.isEmpty()){
        qDebug()<<"Err: Login or Password empty!";
        return;
    }

    foreach(QString i, tempList){
        if(i.split(";")[0].contains(login)){
            qDebug()<<"This account is using";
            return;
        }
    }
    QString newAccount;
    QVector<uintmax_t> openKey = crypted.getOpenKey();
    QString key;
    key = crypted.cript(password,openKey[0], openKey[1]);
    newAccount.append(login + "; " + key);
    stream << "\n";
    stream << newAccount;
    qDebug() << "Added new account";
}
