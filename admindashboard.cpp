#include "admindashboard.h"
#include "ui_admindashboard.h"


AdminDashboard::AdminDashboard(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminDashboard)
{
    ui->setupUi(this);
    //ui->stackedWidget->setCurrentIndex(0);
    //ui->comboBox->setItemData(0, 0, Qt::UserRole - 1);

    ui->stackedWidget->setCurrentIndex(0);

    ui->date->setDateTime(QDateTime::currentDateTime());

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

AdminDashboard::~AdminDashboard()
{
    delete ui;
}

void AdminDashboard::refreshTable() {
    if(!DB.open()) {
        qDebug() << "Failed to open the database.";
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM user_details");
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnWidth(0,60);
    ui->tableWidget->setColumnWidth(1,200);
    ui->tableWidget->setColumnWidth(2,120);
    ui->tableWidget->setColumnWidth(3,200);
    ui->tableWidget->setColumnWidth(4,180);
    ui->tableWidget->setColumnWidth(5,125);

    if (query.exec()) {
        int RowNumber = 0;

        while(query.next())
        {
            ui->tableWidget->insertRow(RowNumber);
            ui->tableWidget->setRowHeight(RowNumber,40);
            ui->tableWidget->setItem(RowNumber, 0, new QTableWidgetItem(QString(query.value("id").toString())));
            ui->tableWidget->setItem(RowNumber, 1, new QTableWidgetItem(QString(query.value("name").toString())));
            ui->tableWidget->setItem(RowNumber, 2, new QTableWidgetItem(QString(query.value("gender").toString())));
            ui->tableWidget->setItem(RowNumber, 3, new QTableWidgetItem(QString(query.value("phone_number").toString())));
            ui->tableWidget->setItem(RowNumber, 4, new QTableWidgetItem(QString(query.value("address").toString())));
            ui->tableWidget->setItem(RowNumber, 5, new QTableWidgetItem(QString(query.value("service").toString())));
            ui->tableWidget->setItem(RowNumber, 6, new QTableWidgetItem(QString(query.value("date").toString())));
            RowNumber++;
        }


    } else {
        // Login failed, show error message
        qDebug() << "Data loading to table error.";
    }

    DB.close();
}

void AdminDashboard::on_logoutBtn_clicked()
{
    LoginPage *loginpage = new LoginPage();
    loginpage->showMaximized();
    this->close();
}


void AdminDashboard::on_dashboardBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void AdminDashboard::on_membersBtn_clicked()
{
    refreshTable();
    ui->stackedWidget->setCurrentIndex(3);
}


void AdminDashboard::on_bookingBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}


void AdminDashboard::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void AdminDashboard::on_pushButton_updateInfo_clicked()
{
    QModelIndexList selectedIndexes = ui->tableWidget->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        qDebug() << "No row selected.";
        QMessageBox::warning(this, "Invalid selection","No row is selected.",QMessageBox::Ok);
        return;
    }
    ui->stackedWidget->setCurrentIndex(2);
    int selectedRow = selectedIndexes.first().row();

    ui->updateid->setText(ui->tableWidget->item(selectedRow, 0)->text());
    ui->updatename->setText(ui->tableWidget->item(selectedRow, 1)->text());
    ui->updatephone_number->setText(ui->tableWidget->item(selectedRow, 3)->text());
    ui->updateaddress->setText(ui->tableWidget->item(selectedRow, 4)->text());

    if(ui->tableWidget->item(selectedRow, 2)->text() == "Male") {
        ui->maleradioButton1->setChecked(true);
    }
    else if(ui->tableWidget->item(selectedRow, 2)->text() == "Female") {
        ui->femaleradioButton1->setChecked(true);
    }
    else if(ui->tableWidget->item(selectedRow, 2)->text() == "Others") {
        ui->othersradioButton1->setChecked(true);
    }
    ui->updateservice->setCurrentText(ui->tableWidget->item(selectedRow, 5)->text());
    ui->updatedate->setDateTime(QDateTime::fromString(ui->tableWidget->item(selectedRow, 6)->text(), "yyyy/M/d h:mm AP"));
}

void AdminDashboard::on_pushButton_addBooking_clicked()
{

    QString name, gender, phone_number, address, service, date;

    name = ui->name->text();
    if(ui->femaleradioButton->isChecked()) {
        gender = ui->femaleradioButton->text();
    }
    else if(ui->maleradioButton->isChecked()) {
        gender = ui->maleradioButton->text();
    }
    else if(ui->othersradioButton->isChecked()) {
        gender = ui->othersradioButton->text();
    }

    phone_number = ui->phone_number->text();
    address = ui->address->text();
    service = ui->service->currentText();
    date = ui->date->text();


    qDebug() << name << gender << phone_number << address << service << date;

    if(!DB.open()) {
        qDebug() << "Failed to open the database.";
        return;
    }


    if(!name.isEmpty() && !gender.isEmpty() && !phone_number.isEmpty() && !address.isEmpty() && !service.isEmpty() && !date.isEmpty()) {
        QSqlQuery query;

        query.prepare("INSERT INTO user_details (name, gender, phone_number, address, service, date) VALUES (:name, :gender, "
                      ":phone_number, :address, :service, :date)");
        query.bindValue(":name", name);
        query.bindValue(":gender", gender);
        query.bindValue(":phone_number", phone_number);
        query.bindValue(":address", address);
        query.bindValue(":service", service);
        query.bindValue(":date", date);

        if (query.exec()) {
            qDebug() << "Data inserted successfully";
            QMessageBox::information(this,"Successful", "User inserted successfully.", QMessageBox::Ok);
            ui->name->clear();
            ui->phone_number->clear();
            ui->address->clear();
            ui->maleradioButton->setChecked(false);
            ui->femaleradioButton->setChecked(false);
            ui->othersradioButton->setChecked(false);
            ui->service->setCurrentIndex(-1);
            ui->date->setDateTime(QDateTime::currentDateTime());
        } else {
            qDebug() << "Error: " << query.lastError().text();
        }
    } else {
        QMessageBox::warning(this,"Booking error", "All fields are required and cannot be empty.", QMessageBox::Ok);
    }

    DB.close();


}

void AdminDashboard::on_refreshtableBtn_clicked()
{
    refreshTable();
}

//delete record
void AdminDashboard::on_pushButton_clicked()
{
    QModelIndexList selectedIndexes = ui->tableWidget->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        qDebug() << "No row selected.";
        QMessageBox::warning(this, "Invalid selection","No row is selected.",QMessageBox::Ok);
        return;
    }

    int selectedRow = selectedIndexes.first().row();

    // Assuming the ID is in the first column (column 0)
    QString idToDelete = ui->tableWidget->item(selectedRow, 0)->text();
    qDebug() << idToDelete;

    if (QMessageBox::question(this, "Confirm Deletion", "Are you sure you want to delete this record?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        // Perform the deletion here using the ID or other identifying information
        if (!DB.open()) {
            qDebug() << "Failed to open the database.";
            return;
        }

        QSqlQuery query;
        query.prepare("DELETE FROM user_details WHERE id = :id");
        query.bindValue(":id", idToDelete);

        if (!query.exec()) {
            qDebug() << "Error deleting record:" << query.lastError().text();
        }

        DB.close();

        // Refresh the table after deletion
        refreshTable();
    }
}


void AdminDashboard::on_pushButton_updateBooking_clicked()
{
    QString name, gender, phone_number, address, service, date, id;

    name = ui->updatename->text();
    if(ui->femaleradioButton1->isChecked()) {
        gender = ui->femaleradioButton1->text();
    }
    else if(ui->maleradioButton1->isChecked()) {
        gender = ui->maleradioButton1->text();
    }
    else if(ui->othersradioButton1->isChecked()) {
        gender = ui->othersradioButton1->text();
    }

    phone_number = ui->updatephone_number->text();
    address = ui->updateaddress->text();
    service = ui->updateservice->currentText();
    date = ui->updatedate->text();
    id = ui->updateid->text();


    if(!DB.open()) {
        qDebug() << "Failed to open the database.";
        return;
    }


    if(!name.isEmpty() && !gender.isEmpty() && !phone_number.isEmpty() && !address.isEmpty() && !service.isEmpty() && !date.isEmpty()) {
        QSqlQuery query;

        query.prepare("UPDATE user_details SET name = :name, gender = :gender, phone_number = :phone_number, address = :address, service = :service, date = :date WHERE id = :id");
        query.bindValue(":name", name);
        query.bindValue(":gender", gender);
        query.bindValue(":phone_number", phone_number);
        query.bindValue(":address", address);
        query.bindValue(":service", service);
        query.bindValue(":date", date);
        query.bindValue(":id",id);

        if (query.exec()) {
            qDebug() << "Data updated successfully";
            QMessageBox::information(this,"Successful", "User information updated successfully.", QMessageBox::Ok);
            ui->updatename->clear();
            ui->updateid->clear();
            ui->updatephone_number->clear();
            ui->updateaddress->clear();
            ui->maleradioButton1->setChecked(false);
            ui->femaleradioButton1->setChecked(false);
            ui->othersradioButton1->setChecked(false);
            ui->updateservice->setCurrentIndex(-1);
            ui->updatedate->setDateTime(QDateTime::currentDateTime());
            refreshTable();
            ui->stackedWidget->setCurrentIndex(3);

        } else {
            qDebug() << "Error: " << query.lastError().text();
        }
    } else {
        QMessageBox::warning(this,"Updating error", "All fields are required and cannot be empty.", QMessageBox::Ok);
    }

    DB.close();
}




