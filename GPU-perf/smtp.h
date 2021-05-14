#ifndef SMTP_H
#define SMTP_H

#include <QtCore>
#include <QCoreApplication>
#include <QObject>
#include <QTcpSocket>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>
#include <QDate>
#include <QLocale>
#include <QMessageBox>


class Smtp : public QObject
{
    Q_OBJECT

public:
    explicit Smtp(QString feedback_name, QString feedback_content);
    ~Smtp();
    bool getSendResult();

private slots:
    void readyReadSlot();

private:
    //检查连接状态
    void checkConnectState();
    //发送helo
    void sendHelo();
    //发送auth login
    void sendAuthLogin();
    //发送用户名
    void sendUser();
    //发送密码
    void sendPassword();
    //发送mail from
    void sendMailFrom();
    //发送rcpt to
    void sendRcptTo();
    //发送data
    void sendData();
    //发送邮件内容
    void sendContent();
    //发送成功后续操作
    void afterSendSuccess();

private:
    QString senderMail; //发送方邮箱地址
    QString authCode; //发送方邮箱授权码
    QString receiveMail; //接收方邮箱地址

    QString title; //邮件标题
    QString content; //邮件正文

    QString expectedReply; //期待收到的应答
    void (Smtp::*nextAction)(); //收到正确应答后下一步要执行的方法

    QTcpSocket *tcpSocket;
    bool status;
};


#endif
