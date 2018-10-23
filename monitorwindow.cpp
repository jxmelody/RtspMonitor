#include "monitorwindow.h"


#include <QPixmap>

MonitorWindow::MonitorWindow(QWidget *parent)
	: QLabel(parent)
{
	m_thread = new RtspThread(this);
    m_usbcamThread = new UsbCamThread(this);

	m_ffmpeg = new QFFmpeg(this);
	m_url = QString();
	m_recordPath =  QString();

	m_thread->setffmpeg(m_ffmpeg);

	m_isRecording = false;
	m_isPlaying = false;
	m_isActive = false;
    m_camType = UNKOWNTYPE;

	this->setStyleSheet(QString("border: 3px solid gray; \
								border-radius: 10px; \
								background: white; \
								background-image: url(:/Resources/background.jpg) 0 0 0 0 stretch stretch; "));


}

MonitorWindow::~MonitorWindow()
{
	if (m_thread)
	{
		m_thread->terminate();
		delete m_thread;
	}

	if (m_ffmpeg)
	{
		delete m_ffmpeg;
	}
}



bool MonitorWindow::isPlaying() const
{
	return m_isPlaying;
}

bool MonitorWindow::isRecording() const
{
	return m_isRecording;
}

int MonitorWindow::getCameraType() const
{
    return m_camType;
}

bool  MonitorWindow::isActive() const
{
	return m_isActive;
}

QFFmpeg* MonitorWindow::ffmpeg() const
{
	return m_ffmpeg;
}

UsbCamThread* MonitorWindow::usbCam() const
{
    return m_usbcamThread;
}

void MonitorWindow::setUrl(QString& url)
{
    m_camType = RTSPCAM;
	m_url = url;
	m_thread->setUrl(m_url);
    m_thread->set_url_form(SDP);
}

void MonitorWindow::setRtspUrl(QString& rtsp_url)
{
    m_camType =RTSPCAM;
    m_url = rtsp_url;
    m_thread->setUrl(m_url);
    m_thread->set_url_form(RTSP);
}

void MonitorWindow::openUSBCam()
{
    m_camType = USBCAM;
    m_usbcamThread->setDevice(0);

}

void MonitorWindow::setOutPath(QString& path, QString& format)
{
	m_recordPath = path;
	m_format = format;
}

void MonitorWindow::startPlaying()
{
    if(m_camType == RTSPCAM) {m_thread->start();}
    else if(m_camType == USBCAM){m_usbcamThread->start();}
	m_isActive = true;
	m_isPlaying = true;
}

void MonitorWindow::pausePlaying()
{   if(m_camType == RTSPCAM) {m_thread->pause();}
    else if(m_camType == USBCAM) {m_usbcamThread->pause();}
	m_isPlaying = false;
}

void MonitorWindow::resumePlaying()
{
    if(m_camType == RTSPCAM) {m_thread->resume();}
    else if(m_camType == USBCAM) {m_usbcamThread->resume();}
    m_isPlaying = true;
}


void MonitorWindow::startRecording()
{
	
	m_isRecording = true;
}

void MonitorWindow::stopRecording()
{
	m_isRecording = false;
}

void MonitorWindow::stop()
{
    if(m_camType == RTSPCAM){
        if (m_thread){
            delete m_thread;
            m_thread = nullptr;
        }
    }
    else if(m_camType == USBCAM){
        if(m_usbcamThread){
            delete m_usbcamThread;
            m_usbcamThread = nullptr;
        }
    }
	m_isPlaying = false;
	m_isRecording = false;
	m_isActive = false;
}

void MonitorWindow::setImage(const QImage& img)
{
	if (img.height() > 0)
	{
        QObject *sender = QObject::sender();
        int wid = this->width();
        int hei = this->height();
        QPixmap pix = QPixmap::fromImage(img.scaled(wid- 10, hei - 10));
		this->setPixmap(pix);
	}
}

