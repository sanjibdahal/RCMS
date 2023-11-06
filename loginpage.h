#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QMainWindow>
#include "registerpage.h"
#include "admindashboard.h"

namespace Ui {
class LoginPage;
}

class LoginPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();

private slots:
    void on_pushButton_clicked();

    void on_loginBtn_clicked();

    void on_signupBtn_clicked();

private:
    Ui::LoginPage *ui;
};

#endif // LOGINPAGE_H
