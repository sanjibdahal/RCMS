#include "loginpage.h"
#include "ui_loginpage.h"
#include <QMessageBox>
#include <QCryptographicHash>

LoginPage::LoginPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginPage)
{
    ui->setupUi(this);
    ui->stackedWidget_2->setCurrentIndex(0);

    QString path_to_database = QCoreApplication::applicationDirPath() + QDir::separator() + "database" + QDir::separator() + "database.db";
    DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName(path_to_database);

    if(DB.open()) {
        qDebug() << "Database connected.";
    }
    else {
        qDebug() << "Database not connected.";
        qDebug() << "Error: " << DB.lastError();
    }
}

LoginPage::~LoginPage()
{
    delete ui;
}

void LoginPage::showMessage(const QString &title, const QString &text, QMessageBox::Icon icon, QFlags<QMessageBox::StandardButton> buttons) {
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setText(text);
    msgBox->setIcon(icon);
    msgBox->setStandardButtons(buttons);
    msgBox->setWindowTitle(title);
    msgBox->setStyleSheet("QLabel{font-size: 18px; color: #fff; font-weight: 400; font-family: 'Poppins';} QPushButton{"
                          "color: #fff; font-family: 'Poppins' }");
    msgBox->exec();
}

bool LoginPage::isValidEmail(const QString &email) {
    // Regular expression for a simple email validation
    QRegularExpression regex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");

    return regex.match(email).hasMatch();
}


void LoginPage::on_loginBtn_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();
    QByteArray hashedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();

    if(!DB.open()) {
        qDebug() << "Failed to open the database.";
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT id FROM credentials WHERE email = :email AND password = :password");
    query.bindValue(":email", username);
//    query.bindValue(":password", password);
    query.bindValue(":password",hashedPassword);

    if (query.exec() && query.next()) {
        // Login successful
        qDebug() << "Login successful";

        // Redirect to another window (you can replace this part with your own logic)
//        this->hide();
        AdminDashboard *admindashboard = new AdminDashboard();
        admindashboard->showMaximized();

        // Close the login window
        close();
    } else {
        // Login failed, show error message
        qDebug() << "Login failed";
        showMessage("Login Error", "Invalid email or password.", QMessageBox::Critical, QMessageBox::Ok);
    }

    DB.close();
}


void LoginPage::on_signupBtn_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(1);
}


void LoginPage::on_loginBtn_2_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}


void LoginPage::on_signupBtn_2_clicked()
{
    QString username = ui->lineEdit_username_2->text();
    QString password = ui->lineEdit_password_2->text();
    QString cpassword = ui->lineEdit_confirm_password->text();

    QByteArray hashedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();

    if(!DB.open()) {
        qDebug() << "Failed to open the database.";
        return;
    }

    if (isValidEmail(username)) {
        qDebug() << "Email is valid!";

        if(password==cpassword && password.length()>=8) {
            QSqlQuery query;
            query.prepare("SELECT * FROM credentials WHERE email = :email");
            query.bindValue(":email", username);

            if (query.exec() && query.next()) {
                // Email is already used, show error message
                qDebug() << "Email already in use";
                showMessage("Registration Error", "This email addess is already in use. Please use another or simply login.",
                            QMessageBox::Critical, QMessageBox::Ok);
            } else {
                query.prepare("INSERT INTO credentials (email, password) VALUES (:email, :password)");
                query.bindValue(":email", username);
//                query.bindValue(":password", password);
                query.bindValue(":password", hashedPassword);

                if (query.exec()) {
                    qDebug() << "Data inserted successfully";
                    this->hide();
                    AdminDashboard *admindashboard = new AdminDashboard();
                    admindashboard->showMaximized();
                } else {
                    qDebug() << "Error: " << query.lastError().text();
                }
            }
        } else {
            // password wrong error thrown
            showMessage("Registration Error", "Password and Confirm password didnot match and password should be at least 8 characters long.", QMessageBox::Warning, QMessageBox::Ok);
        }

    } else {
        qDebug() << "Invalid email!";
        showMessage("Registration Error", "The email addess is not valid. Please enter valid email.", QMessageBox::Critical, QMessageBox::Ok);
    }

    DB.close();

}


