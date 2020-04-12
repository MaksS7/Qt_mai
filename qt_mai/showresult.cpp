#include "showresult.h"
#include "ui_showresult.h"
#include <QFile>
#include <QDebug>

ShowResult::ShowResult(const QStringList &arg1) :
    listTasks(arg1)
    ,ui(new Ui::ShowResult)
{
    ui->setupUi(this);
    ui->comboBox->addItems(listTasks);
}

ShowResult::~ShowResult()
{
    delete ui;
}

void ShowResult::setMatrixNumber(int fRow, int fColumn, double fNumber, QTableWidget* fTable)
{
    QTableWidgetItem* tempTable = new QTableWidgetItem(QString::number(fNumber));
    fTable->setItem(fRow, fColumn, tempTable);
}

void ShowResult::on_btnShow_clicked()
{
    QString nameTemp = ui->comboBox->currentText();
    QFile openedFile(nameTemp + ".ResultAll.txt");
    if (!openedFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"File with this name is not found";
        return;
    }
    QTextStream stream(&openedFile);
    QStringList tempList;
    QStringList name;
    while (!stream.atEnd()) {
        QStringList temp = stream.readAll().split(":");
        name.append(temp[0]);
        temp.removeFirst();
        tempList.append(temp.last());
    }
    for (int i(0); i < tempList.size(); i++) {
        QStringList tempRes = tempList[i].split(";");
        tempRes.removeLast();
        for (int j(0); j < tempRes.size(); j++) {
        }
    }
    ui->tableWidget->setRowCount(name.size());
    ui->tableWidget->setColumnCount(tempList[0].split(";").size()-1);
    ui->tableWidget->setVerticalHeaderLabels(name);
    QStringList columnName;
    for (int i(0); i < tempList.size(); i++) {
        QStringList tempRes = tempList[i].split(";");
        tempRes.removeLast();
        for (int j(0); j < tempRes.size(); j++) {
            if (!i) {
               columnName.append(tempRes[j].split("=")[0]);
            }
            double num = tempRes[j].split("=")[1].toDouble();
            setMatrixNumber(i, j, num, ui->tableWidget);
        }
    }
    ui->tableWidget->setHorizontalHeaderLabels(columnName);
}
