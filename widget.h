#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPalette>
#include <QTextToSpeech>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void analyseWhetherJson(QByteArray array);
    QString JsonObj2String(const QJsonObject jsonObj);

protected:



public slots:
    void finishedSlot(QNetworkReply *reply);
    void finishedSlot2(QNetworkReply *reply);
    void parseCity(QString City);

private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    QNetworkAccessManager *nam;
    QNetworkAccessManager *nam2;
    QNetworkRequest *req;
    QString u1;
    QString u2;
    //QPalette palette;

};

#endif // WIDGET_H
