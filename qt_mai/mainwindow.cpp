#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVector>
#include <QDebug>
#include <QTableWidgetItem>
#include "math.h"
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
}

MainWindow::MainWindow(QString nameProfile)
    : ui(new Ui::MainWindow)
{
    nameAccount = nameProfile;
    ui->setupUi(this);
    if (nameAccount == "Admin"){
        ui->menuAdmin_menu->setEnabled(true);
    }
    tableMain = new QTableWidget;//таблица 1
    tableVariant = new QTableWidget;//таблица 2
    updateTask();
//    repaintTables(strListTask.first(), tableMain, tableVariant);
    connect(this, SIGNAL(mainTableUpdated()),
            this, SLOT(enbleVariantTable()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QVector<QVector<double>> MainWindow::setMatrix(int frow)//получение матрицы n*n
{
    QVector<QVector<double>> _matrix;
    QVector<double> row;
    for(int j(0); j < frow; j++) {
        for(int i(0); i < frow; i++) {
            if( j == i) {//по диагонали 1 осталҗные 0
                row.append(1);
            } else {
                row.append(0);
            }
        }
        _matrix << row;
        row.clear();
    }
    return _matrix;
}

QVector<double> MainWindow::computeSumma(QVector<QVector<double>> fmat)
{
    QVector<double> fsum;
    qDebug()<<fmat;
    int frow = fmat.length();
    for(int col(0); col < frow; col++) {
        double d(0.0);
        for(int row(0); row < frow; row++) {
            d += fmat[row][col];
        }
        fsum.append(d);//запись суммы n столбца
    }
    qDebug() << "fsum" << fsum;
    return fsum;
}

QVector<double> MainWindow::computePercent(QVector<QVector<double>> fmat)
{
    QVector<double> fproizv;//переумножение элементов строк
    int frow = fmat.length();
    for(int row(0); row < frow; row++) {
        double d(fmat[row][0]);
        for(int col(1); col < frow; col++) {
            d *= fmat[row][col];
        }
        fproizv.append(d);
    }
    qDebug() << "proizv" << fproizv;

    double summstep = 0.0;
    QVector<double> fstep;//получения корня из произведения
    for(int i(0); i < frow; i++) {
        fstep.append(pow(fproizv[i], (1.0 / frow)));
        summstep += fstep[i];
    }
    qDebug() << summstep << "step" << fstep;

    QVector<double> fproc;
    for(int i(0); i < frow; i++) {//расчет процента
        fproc.append(fstep[i] / summstep);
    }
    qDebug() << "proc" << fproc;
    return fproc;
}

bool MainWindow::computeIs(int frow, QVector<double> fsum, QVector<double> fproc)
{
    laymba = 0.0;
    for (int i(0); i < frow; i++) {//умножение массива произведений на элементы массива суммы
        laymba += fproc[i] * fsum[i];
    }

    qDebug() << laymba;

    is = fabs(laymba - frow) / (frow - 1);//вычисляем значение ИС
    bool os;
    double ss(0.0);
    //в зависимости от размерности матрицы выбираем соответствующее значение СС
    switch (_mainRow) {
    case 3:
        ss = 0.58;
        break;
    case 4:
        ss = 0.9;
        break;
    case 5:
        ss = 1.12;
        break;
    case 6:
        ss = 1.24;
        break;
    case 7:
        ss = 1.32;
        break;
    case 8:
        ss = 1.41;
        break;
    case 9:
        ss = 1.45;
        break;
    case 10:
        ss = 1.49;
        break;
    default:
        ss = 0.0;
        break;
    }

    if((is/ss) < 0.1) {//проверяем условие согласованости таблицы
        os = true;
    } else {
        os = false;
    }
    qDebug() << "IS: " << is << "os: " << os;
    return os;
}

void MainWindow::setMatrixNumber(int fRow, int fColumn, double fNumber, QTableWidget* fTable)
{
    QTableWidgetItem* tempTable = new QTableWidgetItem(QString::number(fNumber));
    fTable->setItem(fRow, fColumn, tempTable);
}

QVector<QVector<double>> MainWindow::repaintMatrixZeroes(int fRow, QTableWidget* fTable, QVector<QVector<double>> fMatrix)
{
    fTable->setRowCount(fRow);
    fTable->setColumnCount(fRow);
    fMatrix = setMatrix(fRow);
    for(int i(0); i < fRow; i++) {//заполняем таблицу вывода
        setMatrixNumber(i, i, 1, fTable);
    }
    return fMatrix;
}

void MainWindow::repainMatrix(QTableWidget *fTable, QVector<QVector<double> > fMatrix)
{
    fTable->setRowCount(fMatrix.size());
    fTable->setColumnCount(fMatrix.size());
    for(int row(0); row < fMatrix.size(); row++) {
        for(int column(0); column < fMatrix[row].size(); column++) {
            setMatrixNumber(row, column, fMatrix[row][column], fTable);
        }
    }
}

void MainWindow::on_btnUpdateMainTable_clicked()//запись и расчет элементов из таблицы критериев в матрицу
{
    for(int row(0); row < mainMatrix.size(); row++) {
        for(int col(0); col < mainMatrix[row].size(); col++) {
            if(col > row) {
                if(tableMain->item(row,col) != nullptr) {
                    QString str(tableMain->item(row,col)->text());
                    if(str.split("/").size() > 2) {//возможность записи дробеых числе "1/2"
                        QStringList ls = str.split("/");
                        double b = ls[0].toDouble() / ls[1].toDouble();
                        mainMatrix[row][col] = b;
                        mainMatrix[col][row] = 1 / b;
                    } else if(str.length() == 1) {//обычные числа
                        mainMatrix[row][col] = tableMain->item(row, col)->text().toDouble();
                        mainMatrix[col][row] = 1 / tableMain->item(row, col)->text().toDouble();
                    }
                    setMatrixNumber(col, row, mainMatrix[col][row], tableMain);//запись чисел ниже диагонали
                }
            }
        }
    }
    mainSumma = computeSumma(mainMatrix);//сумма элементов в столбце
    mainPercent = computePercent(mainMatrix);//просчет процента важности критерия
    if(computeIs(_mainRow, mainSumma, mainPercent)) {//расчет согласованости элементов матрицы
        emit mainTableUpdated();
        ui->btnSave->setEnabled(true);
        ui->btnReadFileVariant->setEnabled(true);
    } else {
        qDebug() << "Main table IS > 10%, chek table item!";
    }
}

void MainWindow::on_btnUpdateVariantTable_clicked()
{
    for(int row(0); row < _variantRow; row++) {
        for(int col(0); col < _variantRow; col++) {
            if(col > row) {
                if(tableVariant->item(row,col) != nullptr) {
                    QString str(tableVariant->item(row,col)->text());
                    if(str.length() > 2) {
                        QStringList ls = str.split("/");
                        double b = ls[0].toDouble() / ls[1].toDouble();
                        varintMatrix[row][col] = b;
                        varintMatrix[col][row] = 1 / b;
                    } else if(str.length() == 1) {
                        varintMatrix[row][col] = tableVariant->item(row, col)->text().toDouble();
                        varintMatrix[col][row] = 1 / tableVariant->item(row, col)->text().toDouble();
                    }
                    setMatrixNumber(col, row, varintMatrix[col][row], tableVariant);
                }
            }
        }
    }
    allVariantMatrix.append(varintMatrix);
    loop++;
    ui->btnPreviousTableVariant->setEnabled(true);
    if(loop != _mainRow){
        setNumberTableVariant(loop);
    } else {
        tableVariant->setEnabled(false);
        ui->btnUpdateVariantTable->setEnabled(false);
        ui->btnSaveVariant->setEnabled(true);
    }
    tableVariant->clear();
    repaintMatrixZeroes(_variantRow, tableVariant, varintMatrix);
    qDebug() << "Add matrix" << loop << ": Done!";
}

void MainWindow::showMas(QVector<QVector<double>> mas)
{
    for(int i(0); i < mas.length(); i++){
        qDebug() << mas[i].toList();
    }
    qDebug() << endl;
}

void MainWindow::on_btnSave_clicked()
{
    QFile savedFile(nameAccount + "." + nameTarget + ".MainTable.txt");
    if (!savedFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }
    QTextStream stream(&savedFile);
    for(int row(0); row < mainMatrix.length(); row++) {
        for(int column(0); column < mainMatrix[row].length(); column++) {
            stream<<mainMatrix[row][column]<<" ";
        }
        stream<<"\n";
    }
    savedFile.close();
}

void MainWindow::on_btnReadFile_clicked()
{
    QFile savedFile(nameAccount + "." + nameTarget + ".MainTable.txt");
    if (!savedFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"File with this name is not found";
        return;
    }
    QVector<QVector<double>> tempVector;
    QTextStream stream(&savedFile);
    QVector<double> temp;
    while(!stream.atEnd()) {
        QStringList strTemp = stream.readLine().split(" ");
        strTemp.removeLast();
        for(int i(0); i < strTemp.length(); i++){
            temp.append(strTemp[i].toDouble());
        }
        tempVector.append(temp);
        temp.clear();
    }
    qDebug()<<tempVector;
    savedFile.close();
    mainMatrix = tempVector;
    repainMatrix(tableMain, tempVector);
}

void MainWindow::enbleVariantTable()
{
    tableVariant->setEnabled(true);
    ui->btnUpdateVariantTable->setEnabled(true);
    ui->btnSaveVariant->setEnabled(true);
}

void MainWindow::on_btnResultCompute_clicked()
{
    for(int i(0); i < allVariantMatrix.size(); i++) {
        variantSumma = computeSumma(allVariantMatrix[i]);
        variantPercent = computePercent(allVariantMatrix[i]);
        computeIs(_variantRow, variantSumma, variantPercent);
        allPercent.append(variantPercent);//проценты для кажого критерия записываем в массив
    }

    qDebug() << "Final" << endl;
    resultPercent.clear();
    double sumres(0.0);
    for (int j(0); j < _variantRow; j++) {//расчет процента для кажого варианта (ГП1)
        sumres = 0.0;
        for (int i(0); i < _mainRow; i++) {
            sumres += mainPercent[i]*allPercent[i][j];
        }
        qDebug() << "Result: " << sumres;//вывод результата для каждого поставщика
        if(sumres > resultMax){
            resultMax = sumres;
        }
        resultPercent.append(sumres);
    }
    QTableWidgetItem item(*tableVariant->verticalHeaderItem(resultPercent.indexOf(resultMax)));
    ui->lineResult->setText(item.text() + ": " + QString::number(resultMax * 100) + "%");

    QString nameFile = nameTarget + ".ResultAll.txt";
    QFile saveFileResult(nameFile);
    if (!saveFileResult.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append)) {
        return;
    }
    QTextStream stream(&saveFileResult);
    QStringList tempNameList = stream.readAll().split(":");
    if (tempNameList.indexOf(nameAccount)) {
        qDebug() << "Результаты этого аккаунта уже записаны";
        return;
    }
    stream<<nameAccount + ": ";
    foreach (double i, resultPercent) {
        QTableWidgetItem item(*tableVariant->verticalHeaderItem(resultPercent.indexOf(i)));
        stream<<item.text()<<"="<<i<<";";
    }
    stream << "\n";
    saveFileResult.close();
}

void MainWindow::on_btnPreviousTableVariant_clicked()
{
    loop--;
    if(!loop) {
        ui->btnPreviousTableVariant->setEnabled(false);
    }
    ui->btnNextTableVariant->setEnabled(true);
    repainMatrix(tableVariant, allVariantMatrix[loop]);
    setNumberTableVariant(loop);
}

void MainWindow::on_btnNextTableVariant_clicked()
{
    loop++;
    if(loop >= allVariantMatrix.size() || allVariantMatrix[loop].isEmpty()) {
        tableVariant->clear();
        repaintMatrixZeroes(allVariantMatrix[loop - 1].size(), tableVariant, allVariantMatrix[loop - 1]);
        ui->btnNextTableVariant->setEnabled(false);
    } else {
        tableVariant->clear();
        repainMatrix(tableVariant, allVariantMatrix[loop]);
    }
    if(loop + 1>= mainMatrix.size()){
        ui->btnNextTableVariant->setEnabled(false);
        return;
    }
    ui->btnPreviousTableVariant->setEnabled(true);
    setNumberTableVariant(loop);
}

void MainWindow::setNumberTableVariant(int number)
{
    ui->lineEdit_crit->setText(QString::number(number + 1));
    QTableWidgetItem named(*tableMain->horizontalHeaderItem(number));
    ui->lineName->setText(named.text());
}

void MainWindow::repaintTables(const QString &nameTask, QTableWidget *tableMain, QTableWidget *tableVariant)
{
    QFile savedFile("AllTask.txt");
    if (!savedFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"File with this name is not found";
        return;
    }
    QTextStream stream(&savedFile);
    strList.clear();
    strListVar.clear();
    QStringList strTempList = stream.readAll().split(";");
    QString strTemp = nullptr;
    foreach (QString i, strTempList) {
        if(i.split(":")[0] == nameTask) {
            strTemp = i;
            strTempList.clear();
            break;
        }
    }
    if(strTemp.isEmpty()){
        qDebug()<<"Err: Task table empty, please create new task table";
        return;
    }

    strTempList = strTemp.split("\n");
    QStringList tempListMain = strTempList.first().split(":");
    QStringList tempListVariant = strTempList.last().split(":");
    tempListMain.removeLast();
    tempListMain.removeFirst();
    tempListVariant.removeLast();

    ui->labelTarget->setText("Цель: " + nameTask);

    _mainRow = tempListMain.first().toInt();
    tempListMain.removeFirst();

    mainMatrix = repaintMatrixZeroes(_mainRow, tableMain, mainMatrix);//отрисовка таблицы n*n

    foreach (QString i, tempListMain) {
        strList.append(i);
    }
    tableMain->setVerticalHeaderLabels(strList);
    tableMain->setHorizontalHeaderLabels(strList);


    _variantRow = tempListVariant.first().toInt();
    tempListVariant.removeFirst();

    varintMatrix = repaintMatrixZeroes(_variantRow, tableVariant, varintMatrix);//вторая матрица вариантов

    foreach (QString i, tempListVariant) {
        strListVar.append(i);
    }

    tableVariant->setHorizontalHeaderLabels(strListVar);
    tableVariant->setVerticalHeaderLabels(strListVar);

    ui->verticalLayout->addWidget(tableMain);//добавляем таблицы на экран
    ui->verticalLayout_2->addWidget(tableVariant);
    loop = 0;
    QTableWidgetItem named(*tableMain->horizontalHeaderItem(loop));
    ui->lineName->setText(named.text());
    ui->lineEdit_crit->setText(QString::number(loop + 1));
    tableVariant->setEnabled(false);

    tempListVariant.clear();
    tempListMain.clear();
    strTemp.clear();
}

void MainWindow::on_btnLogout_clicked()
{
    emit logout();
}

void MainWindow::on_btnSaveVariant_clicked()
{
    QString nameFile = nameAccount + "." + nameTarget + ".VariantTable.txt";
    QFile savedFile(nameFile);
    if (!savedFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }
    QTextStream stream(&savedFile);
    foreach(auto &i, allVariantMatrix) {
        for(int row(0); row < i.length(); row++) {
            for(int column(0); column < i[row].length(); column++) {
                stream << i[row][column] << " ";
            }
            stream << "\n";
        }
        stream << ";";
    }
    savedFile.close();
}

void MainWindow::on_btnReadFileVariant_clicked()
{
    QString nameFile = nameAccount + "." + nameTarget + ".VariantTable.txt";
    QFile readedFile(nameFile);
    if (!readedFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"File with this name is not found";
        return;
    }
    QVector<QVector<double>> tempVector;
    QTextStream stream(&readedFile);
    QVector<double> temp;
    QStringList strTemp = stream.readAll().split(";");
    strTemp.removeLast();
    allVariantMatrix.clear();

    QVector<double> rowVector;
    QVector<QVector<double>> matrixFromFile;

    foreach(QString i, strTemp) {
        QStringList stringSplitList = i.split("\n");
        stringSplitList.removeLast();
        foreach(QString i, stringSplitList) {
            QStringList rowList = i.split(" ");
            rowList.removeLast();
            foreach(QString i, rowList) {
                rowVector.append(i.toDouble());
            }
            matrixFromFile.append(rowVector);
            rowVector.clear();
        }
        allVariantMatrix.append(matrixFromFile);
        matrixFromFile.clear();
    }
    readedFile.close();
    loop = allVariantMatrix.size() - 1;
    repainMatrix(tableVariant, allVariantMatrix[loop]);
    setNumberTableVariant(loop);
    ui->btnPreviousTableVariant->setEnabled(true);
    ui->btnResultCompute->setEnabled(true);
    ui->btnUpdateVariantTable->setEnabled(false);
}

void MainWindow::on_actionAdd_new_task_triggered()
{
    newTask = new NewTask;
    newTask->show();
    connect(newTask, SIGNAL(saveDone()),
            this, SLOT(updateTask()));
}

void MainWindow::updateTask()
{
    QFile savedFile("AllTask.txt");
    if (!savedFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"File with this name is not found";
        return;
    }
    strListTask.clear();
    QTextStream stream(&savedFile);
    QStringList strTemp = stream.readAll().split(";");
    foreach (QString i , strTemp) {
        strListTask.append(i.split(":")[0]);
    }
    strListTask.removeLast();
    ui->comboBoxTask->addItems(strListTask);
}

void MainWindow::on_comboBoxTask_currentIndexChanged(const QString &arg1)
{
    nameTarget = arg1;
    repaintTables(arg1, tableMain, tableVariant);
}

void MainWindow::on_actionShow_results_triggered()
{
    showResult = new ShowResult(strListTask);
    showResult->show();
}

void MainWindow::on_btnShowTaskTable_clicked()
{

}
