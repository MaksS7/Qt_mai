#include "newtask.h"
#include "ui_newtask.h"
#include <QDebug>
#include <QFile>

NewTask::NewTask(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewTask)
{
    ui->setupUi(this);
//    tableMain = new QTableWidget;
    connect(this, SIGNAL(getTable()),
            this, SLOT(createTable()));
}

NewTask::~NewTask()
{
    delete ui;
}

void NewTask::on_btnNextStep_clicked()
{
    if (!size) {
        size = ui->lineTableSize->text().toInt();
        ui->lineTableSize->setText("");
        loop = 0;
        ui->labelSize->setText("Criter " + QString::number(loop + 1));
        return;
    }
    if(loop < size){
        strList.append(ui->lineTableSize->text());
    }
    if(loop + 1 == size){
        ui->btnNextStep->setEnabled(false);
        ui->btnNextStepVarint->setEnabled(true);
        ui->lineTableSize->setEnabled(false);
        ui->lineTableSizeVariant->setEnabled(true);
    } else {
        loop++;
        ui->labelSize->setText("Criter " + QString::number(loop + 1));
    }
    ui->lineTableSize->setText("");
}

void NewTask::on_btnSaveTable_clicked()
{
    qDebug()<<size<<strList;
    qDebug()<<sizeVariant<<strListVariant;
    QFile saveFile("AllTask.txt");
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        return;
    }
    QTextStream stream(&saveFile);

    stream<<nameTask<<":"<<size<<":";
    foreach(QString i, strList){
        stream<<i<<":";
    }
    stream<<"\n";

    stream<<sizeVariant<<":";
    foreach(QString i, strListVariant){
        stream<<i<<":";
    }
    stream<<";";

    emit saveDone();
}

void NewTask::on_btnNextStepVarint_clicked()
{
    if (!sizeVariant) {
        sizeVariant = ui->lineTableSizeVariant->text().toInt();
        ui->lineTableSizeVariant->setText("");
        loop = 0;
        ui->labelSizeVariant->setText("Variant " + QString::number(loop + 1));
        return;
    }
    if(loop < sizeVariant){
        strListVariant.append(ui->lineTableSizeVariant->text());
    }
    if(loop + 1 == sizeVariant){
        ui->btnNextStepVarint->setEnabled(false);
        ui->lineTableSizeVariant->setEnabled(false);
        ui->btnSaveTable->setEnabled(true);
        emit getTable();
    } else {
        loop++;
        ui->labelSizeVariant->setText("Variant " + QString::number(loop + 1));
    }
    ui->lineTableSizeVariant->setText("");
}

void NewTask::on_btnCreatTask_clicked()
{
    nameTask = ui->lineCreateTaskName->text();
    ui->btnNextStep->setEnabled(true);
}

void NewTask::createTable()
{
    qDebug()<<size<<sizeVariant;
    if(!(sizeVariant == 0 && size == 0)) {
        ui->tableWidget->setRowCount(sizeVariant);
        ui->tableWidget->setColumnCount(size);
        ui->tableWidget->setHorizontalHeaderLabels(strList);
        ui->tableWidget->setVerticalHeaderLabels(strListVariant);
    }
}
