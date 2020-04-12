#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "newtask.h"
#include "showresult.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    MainWindow(QString nameProfile);
    ~MainWindow();
    QTableWidget* tableMain;
    QTableWidget* tableVariant;
    QVector<QVector<double>> setMatrix(int frow);
    QVector<double> computeSumma(QVector<QVector<double>> fmat);
    QVector<double> computeMultiplier();
    QVector<double> computePercent(QVector<QVector<double> > fmat);
    bool computeIs(int frow, QVector<double> fsum, QVector<double> fproc);
    void setMatrixNumber(int fRow, int fColumn, double fNumber, QTableWidget *fTable);
    QVector<QVector<double> > repaintMatrixZeroes(int fRow, QTableWidget *fTable, QVector<QVector<double> > fMatrix);
    void repainMatrix(QTableWidget *fTable,QVector<QVector<double> > fMatrix);
    double laymba;
    double is;
    QVector<QVector<double>> mainMatrix;
    QVector<QVector<double>> allPercent;
    QVector<QVector<double>> varintMatrix;
    QVector<double> mainSumma;
    QVector<double> variantSumma;
    QVector<double> mainPercent;
    QVector<double> variantPercent;
    QVector<double> resultPercent;
    QString nameAccount;
    QStringList strListVar;
    QStringList strList;
    QStringList strListTask;
    QString nameTarget;
    int _mainRow;
    int _variantRow;
    int loop = 0;
    double resultMax = 0;
    QVector<QVector<QVector<double>>> allVariantMatrix;
    void showMas(QVector<QVector<double> > mas);

private slots:
    void on_btnUpdateMainTable_clicked();
    void on_btnUpdateVariantTable_clicked();
    void on_btnSave_clicked();
    void on_btnReadFile_clicked();
    void enbleVariantTable();
    void on_btnResultCompute_clicked();
    void on_btnPreviousTableVariant_clicked();
    void on_btnNextTableVariant_clicked();
    void on_btnLogout_clicked();
    void on_btnSaveVariant_clicked();
    void on_btnReadFileVariant_clicked();
    void on_actionAdd_new_task_triggered();
    void updateTask();
    void on_comboBoxTask_currentIndexChanged(const QString &arg1);
    void on_actionShow_results_triggered();

    void on_btnShowTaskTable_clicked();

signals:
    void mainTableUpdated();
    void logout();

private:
    Ui::MainWindow *ui;

protected:
    NewTask *newTask;
    ShowResult *showResult;
    void setNumberTableVariant(int number);
    void repaintTables(const QString &nameTask, QTableWidget *tableMain, QTableWidget *tableVariant);
};

#endif // MAINWINDOW_H
