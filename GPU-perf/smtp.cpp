#include "smtp.h"
#include <QTcpSocket>
#include <QMessageBox>

#pragma execution_character_set("utf-8")

Smtp::Smtp(QString feedback_name, QString feedback_content):
    senderMail("eric_dma@163.com"), authCode("BLSEXUKHFKLGKENQ"), receiveMail("eric@sietium.com"),
    expectedReply("220 163.com Anti-spam GT for Coremail System"), nextAction(&Smtp::checkConnectState)
{
    title = "GPUPerfTool FeedBack From" + feedback_name;
    content = feedback_content;
    status = false;

    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost("smtp.163.com", 25);
    connect(tcpSocket, &QTcpSocket::readyRead,this, &Smtp::readyReadSlot);
}

Smtp::~Smtp()
{
}

void Smtp::readyReadSlot()
{
    QByteArray buffer = tcpSocket->readAll();

#ifdef DEBUG
    qDebug()<<"收到服务器回复："<<buffer;
#endif

    if(buffer.contains(this->expectedReply.toLatin1())){
        if(nextAction) (this->*nextAction)();
    }
    else{
#ifdef DEBUG
        qDebug()<<"邮件发送失败!";
#endif
        status = false;
    }
}

//检查连接状态
void Smtp::checkConnectState()
{
    if(tcpSocket->state() == QAbstractSocket::ConnectedState){
        sendHelo();
    }
    else{
#ifdef DEBUG
        qDebug()<<"连接失败！";
#endif
        status = false;
    }
}

//发送helo
void Smtp::sendHelo()
{
    QString str = "helo 163.com\r\n";
#ifdef DEBUG
    qDebug()<<"向服务器发送：　" + str;
#endif
    tcpSocket->write(str.toLatin1());
    this->expectedReply = "250 OK\r\n";
    this->nextAction = &Smtp::sendAuthLogin;
}

//发送auth login
void Smtp::sendAuthLogin()
{
    QString str = "auth login\r\n";
#ifdef DEBUG
    qDebug()<<"向服务器发送：　" + str;
#endif
    tcpSocket->write(str.toLatin1());
    this->expectedReply = "334 dXNlcm5hbWU6\r\n";
    this->nextAction = &Smtp::sendUser;
}

//发送用户名
void Smtp::sendUser()
{
    QString str = QString("\r\n").prepend(senderMail.toLatin1().toBase64());
#ifdef DEBUG
    qDebug()<<"向服务器发送：　" + str;
#endif
    tcpSocket->write(str.toLatin1());
    this->expectedReply = "334 UGFzc3dvcmQ6\r\n";
    this->nextAction = &Smtp::sendPassword;
}

//发送密码
void Smtp::sendPassword()
{
    QString str = QString("\r\n").prepend(authCode.toLatin1().toBase64());
#ifdef DEBUG
    qDebug()<<"向服务器发送：　" + str;
#endif
    tcpSocket->write(str.toLatin1());
    this->expectedReply = "235 Authentication successful\r\n";
    this->nextAction = &Smtp::sendMailFrom;
}

//发送mail from
void Smtp::sendMailFrom()
{
    QString str = QString("mail from: <%1>\r\n").arg(senderMail);
#ifdef DEBUG
    qDebug()<<"向服务器发送：　" + str;
#endif
    tcpSocket->write(str.toLatin1());
    this->expectedReply = "250 Mail OK\r\n";
    this->nextAction = &Smtp::sendRcptTo;
}

//发送rcpt to
void Smtp::sendRcptTo()
{
    QString str = QString("rcpt to: <%1>\r\n").arg(receiveMail);
#ifdef DEBUG
    qDebug()<<"向服务器发送：　" + str;
#endif
    tcpSocket->write(str.toLatin1());
    this->expectedReply = "250 Mail OK\r\n";
    this->nextAction = &Smtp::sendData;
}

//发送data
void Smtp::sendData()
{
    QString str = "data\r\n";
#ifdef DEBUG
    qDebug()<<"向服务器发送：　" + str;
#endif
    tcpSocket->write(str.toLatin1());
    this->expectedReply = "354 End data with <CR><LF>.<CR><LF>\r\n";
    this->nextAction = &Smtp::sendContent;
}

//发送邮件内容
void Smtp::sendContent()
{
    QString str = QString("from:%1\r\n"
                          "to:%2\r\n"
                          "subject:=?UTF-8?B?%3?=\r\n"
                          "\r\n"
                          "%4\r\n"
                          "\r\n"
                          ".\r\n").arg(senderMail).arg(receiveMail).arg( QString("").append(title.toUtf8().toBase64()) ).arg(content);

#ifdef DEBUG
    qDebug()<<"向服务器发送：　" + str;
#endif
    tcpSocket->write(str.toUtf8());
    this->expectedReply = "250 Mail OK queued as";
    this->nextAction = &Smtp::afterSendSuccess;
}

//发送成功后续操作
void Smtp::afterSendSuccess()
{
#ifdef DEBUG
    qDebug()<<"邮件发送成功!";
#endif
    status = true;
}

// get sent status
bool Smtp::getSendResult()
{
    return status;
}
