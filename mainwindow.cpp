#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //qDebug() << QCoreApplication::applicationDirPath();
    //qDebug() << QCoreApplication::applicationFilePath();
    QString directoryPath = QCoreApplication::applicationDirPath() + QDir::separator() + "database";
    QString databasePath = directoryPath + QDir::separator() + "database.db";

    // Check if the directory exists
    QDir directory(directoryPath);
    if (!directory.exists()) {
        // Create the directory
        if (directory.mkpath(".")) {
            qDebug() << "Directory created successfully.";
        } else {
            qWarning() << "Failed to create the directory.";
        }
    } else {
        qDebug() << "Directory already exists.";
    }

    // Check if the database file exists
    QFile databaseFile(databasePath);
    if (!databaseFile.exists()) {
        // Create the database file
        if (databaseFile.open(QIODevice::ReadWrite)) {
            qDebug() << "Database file created successfully.";
            databaseFile.close();
        } else {
            qWarning() << "Failed to create the database file.";
        }
    } else {
        qDebug() << "Database file already exists.";
    }

    // Set up the SQLite database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databasePath);

    if (!db.open()) {
        qWarning() << "Failed to open database:" << db.lastError().text();
    } else {
        qDebug() << "Database opened successfully.";
    }

    // Create tables if they do not exist

    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS credentials (id INTEGER PRIMARY KEY AUTOINCREMENT, email TEXT NOT NULL UNIQUE, password TEXT NOT NULL);");

    if (query.exec()) {
        qDebug() << "Table credentials created successfully.";
    } else {
        qWarning() << "Failed to create table credentials" << ":" << query.lastError().text();
    }

    query.prepare("CREATE TABLE IF NOT EXISTS user_details (id INTEGER PRIMARY KEY AUTOINCREMENT, name	TEXT NOT NULL, gender TEXT NOT NULL, "
                  "phone_number TEXT NOT NULL, address TEXT NOT NULL, service TEXT NOT NULL, date TEXT NOT NULL, time TEXT NOT NULL);");

    if (query.exec()) {
        qDebug() << "Table user_details created successfully.";
    } else {
        qWarning() << "Failed to create table user_details"<< ":" << query.lastError().text();
    }

    db.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    this->hide();
    LoginPage *loginpage = new LoginPage();
    loginpage->showMaximized();

}

