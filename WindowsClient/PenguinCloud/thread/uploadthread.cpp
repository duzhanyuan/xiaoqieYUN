#include "uploadthread.h"


UploadThread::UploadThread(QString localPath, QString remotePath, QObject *parent):
    UpdateFileThread(localPath, remotePath, parent)
{
    QFileInfo fileInfo(localPath);

    if(!fileInfo.isFile())
    {
        throw fileInfo.fileName() + "not a File!";
    }

    if(!fileInfo.exists())
    {
        throw fileInfo.filePath() + ": not exits";
    }
    else
    {
        m_file.setFileName(fileInfo.filePath());
        if(m_file.open(QIODevice::ReadOnly))
        {
            m_i64FileSize = qint64(m_file.size());
            qDebug() << "m_i64FileSize" << m_i64FileSize;
        }
    }
}

UploadThread::~UploadThread()
{

}

void UploadThread::stopCurrenTask()
{
    m_bFinished = true;
    //删除已经上传的文件
}

void UploadThread::pauseCurrenTask()
{
    qDebug() << "pause";
    m_bFinished = true;

}

void UploadThread::loadDataFromFile(ConnectToFileServer *server, const UploadMsg &uploadMsg)
{
    //文件偏移到当前位置
    m_file.seek(m_i64CurrentSize);

    QDataStream dataStream(&m_file);
    dataStream.setVersion(QDataStream::Qt_5_6);

	char *sendBuf = new char[FILE_SEND + 1];
	memset(sendBuf, 0, FILE_SEND);

    int ret = 0;

    while(!dataStream.atEnd() && !m_bFinished)
    {
		ret = dataStream.readRawData(sendBuf, FILE_SEND);

        qDebug() << m_i64CurrentSize;

        m_i64CurrentSize += qint64(ret);

        server->write(sendBuf, ret);

        memset(sendBuf, 0, ret);
    }

    server->waitForReadyRead();

    delete []sendBuf;
    m_bFinished = true;
}

void UploadThread::run()
{
    m_pSocket = new ConnectToFileServer(m_serverUrl);

    UploadMsg uploadMsg;

    memset(&uploadMsg, 0, sizeof(UploadMsg));
    strcpy(uploadMsg.fileName, m_stRemotePath.toUtf8().data());
    QFileInfo fileInfo(m_stLocalPath);
    strcat(uploadMsg.fileName, fileInfo.fileName().toUtf8().data());

    m_pSocket->sendUploadMsg(uploadMsg);


    while(!m_bFinished && m_pSocket->isConnected())
    {
        if(m_pSocket->waitForReadyRead())
        {
            QByteArray byteArray = m_pSocket->readAll();
            //翻译buf
            Msg *msg = (Msg *)byteArray.data();

            UploadMsg uploadMsg;
            memcpy(&uploadMsg, msg->m_aMsgData, msg->m_iMsgLen);

            loadDataFromFile(m_pSocket, uploadMsg);
        }
        else
        {
            qDebug() << "任务超时";
            m_bFinished = true;
        }
    }

    delete m_pSocket;
    m_pSocket = nullptr;
}


