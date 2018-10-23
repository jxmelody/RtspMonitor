#include "usbcamthread.h"

UsbCamThread::UsbCamThread(QObject* parent)
    : QThread(parent)
{

}

void UsbCamThread::setDevice(int device)
{
    m_device = device;
}

void UsbCamThread::run()
{
    cv::Mat cvImage;
    QImage qImage;
    cv::VideoCapture cap;
    cap.open(m_device);
    if(!cap.isOpened())
    {
        printf("Cannot open camera %d",m_device);
        return;
    }
    while(true)
    {
        m_playMutex.lock();
        cap>>cvImage;
        cv::cvtColor(cvImage, cvImage, CV_BGR2RGB);
        int video_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
        int video_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
        switch (cvImage.type()) {
        case CV_8UC4:
            qImage = QImage(cvImage.data, video_width, video_height,cvImage.step, QImage::Format_RGB32 );
            break;
        case CV_8UC3:
            qImage = QImage(cvImage.data, video_width, video_height,cvImage.step, QImage::Format_RGB888 );
            break;
        default:
            printf("don't exactly what the type is\n");
            break;
        }
        emit getUsbCamImage(qImage);
        m_playMutex.unlock();
    }
}


void UsbCamThread::pause()
{
    m_playMutex.lock();
}

void UsbCamThread::resume()
{
    m_playMutex.unlock();
}
