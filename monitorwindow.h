#ifndef MONITORWINDOW_H
#define MONITORWINDOW_H

#include <QObject>
#include <QLabel>
#include <QString>
#include "rtspthread.h"
#include "qffmpeg.h"
#include "usbcamthread.h"

//class QFFmpeg;
//class RtspThread;

#define UNKOWNTYPE 0
#define USBCAM 1
#define RTSPCAM 2

//���Ӵ��ڣ���Ƶͼ�����ʾ�ڼ��Ӵ�����
class MonitorWindow : public QLabel
{
	Q_OBJECT

public:
	MonitorWindow(QWidget *parent);
	~MonitorWindow();

public slots:
    //�жϴ����Ƿ��ڲ�����
	bool isPlaying() const;
	//�жϴ����Ƿ���¼����
	bool isRecording() const;
	//�жϴ����Ƿ���ڻ״̬
	bool isActive() const;
	//��ô��ڵĽ�����
	QFFmpeg* ffmpeg() const;
    UsbCamThread* usbCam() const;
    void openUSBCam();
	//���ô��ڵ�url·��
	void setUrl(QString& url);
    void setRtspUrl(QString& rtsp_url);
	//���ô�����Ƶ��¼���ļ������·��
	void setOutPath(QString& path,QString& format);
	//��ʼ����
	void startPlaying();
	//��ͣ����
	void pausePlaying();
	//��������
	void resumePlaying();
	//��ʼ¼��
	void startRecording();
	//ֹͣ¼��
	void stopRecording();
	//�رղ��ź�¼�ƣ���ʱisActive�᷵��false
	void stop();
	//��һ֡ͼ�����õ���������ʾ
	void setImage(const QImage& img);
    //different camera type should use different thread
    int getCameraType() const;

private:

	RtspThread *m_thread;
    UsbCamThread *m_usbcamThread;
	QFFmpeg *m_ffmpeg;
	QString m_url;
	QString m_recordPath;
	QString m_format;
    int m_camType;
	bool m_isRecording;
	bool m_isPlaying;
	bool m_isActive;
	
};

#endif // MONITORWINDOW_H
