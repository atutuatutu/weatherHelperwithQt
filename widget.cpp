#include "widget.h"
#include "ui_widget.h"
#include<QDebug>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QMessageBox>
#include<QThread>
#include<QDateTime>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setFixedSize(540,510);
    setWindowTitle("天气助手");
    QDateTime time = QDateTime::currentDateTime();
    ui->date->setText(tr("%1").arg(time.toString("yyyy-MM-dd")));
    ui->widget1->setGeometry(0,0,width(),height());
    u1="http://wthrcdn.etouch.cn/weather_mini?city=";
    nam=new QNetworkAccessManager(this);
    nam2=new QNetworkAccessManager(this);
    QObject::connect(nam2, SIGNAL(finished(QNetworkReply*)),
                 this, SLOT(finishedSlot2(QNetworkReply*)));
   // QThread::msleep(20);
    QObject::connect(nam, SIGNAL(finished(QNetworkReply*)),
                 this, SLOT(finishedSlot(QNetworkReply*)));
    QUrl url2("http://int.dpool.sina.com.cn/iplookup/iplookup.php");
    QNetworkReply* reply2 = nam2->get(QNetworkRequest(url2));
    QThread::msleep(20);
    QUrl url1(u1);
    //qDebug()<<u1<<"Widget";
    //QNetworkReply* reply = nam->get(QNetworkRequest(url1));

}

Widget::~Widget()
{
    delete ui;
}

void Widget::finishedSlot(QNetworkReply *reply)
{
    QByteArray array=reply->readAll();
    //qDebug()<<array.data();
    reply->deleteLater();
    analyseWhetherJson(array);
}

void Widget::finishedSlot2(QNetworkReply *reply)
{
    QByteArray array=reply->readAll();
    QString str = QString::fromLocal8Bit(array);
    reply->deleteLater();
    parseCity(str);

}

void Widget::analyseWhetherJson(QByteArray array)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(array);
    QJsonObject jsonObj= jsonDoc.object().value("data").toObject();
    QJsonArray forecast = jsonObj.value("forecast").toArray();
    ui->textBrowserganmao->setText(jsonObj.value("ganmao").toString());
    //qDebug()<<forecast;
    QJsonObject today=forecast[0].toObject();
    if(today.value("type").toString()=="多云")
    {
        qDebug()<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
        ui->widget1->setStyleSheet("#widget1{border-image: url(:/多云.png);}");
    }
    if(today.value("type").toString()=="晴")
    {
        qDebug()<<"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
        ui->widget1->setStyleSheet("#widget1{border-image: url(:/晴天.png);}");
    }
    if(today.value("type").toString()=="小雨"||
            jsonObj.value("type").toString()=="中雨"||
            jsonObj.value("type").toString()=="大雨"||
            jsonObj.value("type").toString()=="暴雨"||
            jsonObj.value("type").toString()=="大暴雨")
    {
        ui->widget1->setStyleSheet("#widget1{border-image: url(:/雨天.png);}");
    }
    if(today.value("type").toString()=="小雪"||
            jsonObj.value("type").toString()=="中雪"||
            jsonObj.value("type").toString()=="大雪"||
            jsonObj.value("type").toString()=="暴雪"||
            jsonObj.value("type").toString()=="大暴雪"||
            jsonObj.value("type").toString()=="阵雪")
    {
        qDebug()<<"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
        ui->widget1->setStyleSheet("#widget1{border-image: url(:/雪天.png);}");
    }
    ui->textBrowsertoday->setText(JsonObj2String(today));
    QJsonObject tomorrow =forecast[1].toObject();
    ui->textBrowsertomorrow->setText(JsonObj2String(tomorrow));
    QJsonObject aftertomorrow=forecast[2].toObject();
    ui->textBrowseraftertomorrow->setText(JsonObj2String(aftertomorrow));
}

QString Widget::JsonObj2String(QJsonObject jsonObj)
{
    QString weather;
    if(!jsonObj.isEmpty())
    {
        weather += jsonObj.value("date").toString()          + "\n";
        weather += jsonObj.value("type").toString()          + "\n";
        if(!jsonObj.value("fx").toString().isEmpty())
            weather += jsonObj.value("fx").toString()        + "\n";
        else
            weather += jsonObj.value("fengxiang").toString() + "\n";
        weather += jsonObj.value("high").toString()          + "\n";
        weather += jsonObj.value("low").toString()           + "\n";
        //weather += jsonObj.value("type").toString();
    }
    //palette =this->palette;
    return weather;
}

void Widget::parseCity(QString City)
{
    QString city = City.split(tr("	")).at(5);
    if(city == "")
    {
        QMessageBox::information(this,tr("提示"),tr("无法定位城市,当前网络环境不支持定位"),QMessageBox::Ok,QMessageBox::Ok);
        return ;
    }
    //qDebug()<<city;
    u1+=city;
    //qDebug()<<u1<<"parseCity";
    QUrl url1(u1);
    QNetworkReply* reply = nam->get(QNetworkRequest(u1));
    ui->label_4->setText(("当前城市")+city);
}


void Widget::on_pushButton_clicked()
{
    QTextToSpeech *tts = new QTextToSpeech(this);
    tts->say("天气助手为您播报\n"+ui->label_4->text());
    tts->say("今日天气\n"+ui->textBrowsertoday->toPlainText());
    tts->say("温馨提示\n"+ui->textBrowserganmao->toPlainText());
    tts->say("明日天气\n"+ui->textBrowsertomorrow->toPlainText());
    tts->say("后日天气"+ui->textBrowseraftertomorrow->toPlainText());
}
