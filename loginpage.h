#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QMainWindow>
#include "registerpage.h"
#include "admindashboard.h"
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>
#include <QRegularExpression>

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

    void on_loginBtn_2_clicked();

    void on_signupBtn_2_clicked();

    void showMessage(const QString &title, const QString &text, QMessageBox::Icon icon = QMessageBox::Information, QFlags<QMessageBox::StandardButton> buttons = QMessageBox::Ok);

    bool isValidEmail(const QString &email);

    void on_loginBtn_pressed();

private:
    Ui::LoginPage *ui;
    QSqlDatabase DB;
};

#endif // LOGINPAGE_H
