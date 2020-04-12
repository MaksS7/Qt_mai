#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "mainwindow.h"
#include "encrypt.h"

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    QString login;
    ~Login();

private slots:
    void on_btnLogin_clicked();
    void on_btnExit_clicked();

signals:
    bool isKey();

private:
    MainWindow *m;
    Ui::Login *ui;

protected:
    QString password;
    Encrypt crypted;

protected slots:
    void openWindow();
    void addNewAccount();
//    void keyDone();
};

#endif // LOGIN_H
