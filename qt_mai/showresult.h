#ifndef SHOWRESULT_H
#define SHOWRESULT_H

#include <QWidget>
#include <QTableWidget>

namespace Ui {
class ShowResult;
}

class ShowResult : public QWidget
{
    Q_OBJECT

public:
    explicit ShowResult(const QStringList &arg1);
    ~ShowResult();

private slots:
    void on_btnShow_clicked();

private:
    void setMatrixNumber(int fRow, int fColumn, double fNumber, QTableWidget* fTable);
    QStringList listTasks;
    Ui::ShowResult *ui;
};

#endif // SHOWRESULT_H
