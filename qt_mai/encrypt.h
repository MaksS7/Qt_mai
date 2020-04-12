#ifndef RSA_H
#define RSA_H
#include <QString>
#include <cmath>
#include <QDebug>

class Encrypt
{
public:
    Encrypt();
    QVector<uintmax_t> OpenKey;
    QVector<uintmax_t> getOpenKey();
    double getP();
    double getQ();
    double getN();
    uint64_t crip;
    uint64_t uncrip;
    uint64_t cript(uint64_t a, uint64_t e, uint64_t mod = ULONG_MAX);
    QString cript(QString tempStr, uint64_t e, uint64_t mod = ULONG_MAX);
    uint64_t decript(uint64_t m);
    QString decript(QStringList str);


protected:
    QVector<uintmax_t> HideKey;
    double p;
    double q;
    double n;
    double fi;
    uint64_t d;
    double m;
    double e;
    QString engText = "the treasure is buried near the house";
};

#endif // RSA_H
