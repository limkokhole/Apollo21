/*
        Project: Apollo 21
        Filename: vehicle.c
        Author: Ömer Furkan
        Contact: omer.furkan.uyanik1@gmail.com
        Date: 12 Kasım 2016 - 15:47

        x86 BUILD
        gcc vehicle.c -z execstack -m32 -w -fno-stack-protector -o vehicle -lpthread
        x64 BUILD
        gcc vehicle.c -z execstack -w -fno-stack-protector -o vehicle -lpthread

    Marsa gönderilecek aracın özelliklerini ve iletişimini sağlayacak işlemlerin
    bulunduğu kaynak dosyası.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>


#define CONTROL_DEVICE          "127.0.0.1"
#define COMMUNICATION_CH        0x539
#define VEHICLE_NAME            "Apollo 21"

#define Clear_Screen()          printf("\033[H\033[J")



typedef struct
{
    char   _cVehicleName[0x40];
    int    _iFuel;
    int    _iFuelStatus;
    int    _iLocationX;
    int    _iLocationY;
    int    _iSpeed;
    float  _fGone;
}Apollo21_t;


typedef struct
{
    int _iDistance;
    
}Mars_t;



Apollo21_t *Apollo21;
Mars_t *Mars;

int _iSocketFD;
struct sockaddr_in _sSCommDev;
char _cRecvCommand[0x400];
char _cSendData[0x400];
bool _bIsDepature = false;
bool _bControl = false;
pthread_t _pThread, _pThread1;


int _iGenerateRandomNumber(int _iMax)
{
    sleep(0x1);
    time_t _tT;
    srand((unsigned) time(&_tT));
    return rand() % _iMax;
}



void *_vUFODetect()
{
    int _iX = _iGenerateRandomNumber(0xFF);
    int _iY = _iGenerateRandomNumber(0xFF-0x1);

    if(Apollo21->_iLocationX == _iX && Apollo21->_iLocationY == _iY)
    {
        sprintf(_cSendData, "Bilinmeyen Bir Cisimle Karşılaşıldı... Ne Yapmak İstersiniz?\n1- Kaç\n2-İletişime Geç");
        write(_iSocketFD, _cSendData, sizeof(_cSendData));
    }
}



void *_vRoadCalc()
{
    int     _iDirection = 0x1, _iMaxSpeed = 0xFF, _iTime = 0x0, _iCount = 0x0;
    float   _fGone = 0x0;

    while((float)Mars->_iDistance > Apollo21->_fGone)
    {
        if((float)Mars->_iDistance - Apollo21->_fGone < _fGone)
        {
            _iDirection = 0x0;
        }

        if(_iDirection == 0x1)
        {
            if(Apollo21->_iSpeed < _iMaxSpeed)
            {
                Apollo21->_iSpeed = _iTime * _iTime;
            }
            else if(Apollo21->_iSpeed > _iMaxSpeed)
            {
                Apollo21->_iSpeed = _iMaxSpeed;
                if(_fGone == 0x0 || _iCount == 0x0)
                {
                    _fGone = Apollo21->_fGone;
                    _iCount = _iTime - 0x1;
                }
            }
        }

        if(_iDirection == 0x0)
        {
            if(Apollo21->_iSpeed > 1)
            {
                _iCount--;
                Apollo21->_iSpeed = _iCount * _iCount;
            }
        }

        Apollo21->_fGone += Apollo21->_iSpeed;
        printf("Hız: %dkm/s Yol %0.2fkm\n", Apollo21->_iSpeed, Apollo21->_fGone);

        _iTime++;
        sleep(0x1);
    }

    Apollo21->_iLocationX = _iGenerateRandomNumber(0xFF);
    Apollo21->_iLocationY = _iGenerateRandomNumber(0xFF-0x1);
    _bControl = true;
    pthread_exit(0x0);
}



void *_vFuelCalc()
{
    int _iX;
    for(_iX = 0x0; _iX <= 0x12C; _iX++)
    {
        sleep(0x3);
        if(Apollo21->_iFuel >= 0x0)
        {
            Apollo21->_iFuel = Apollo21->_iFuel - 0x1;
            if(Apollo21->_iFuel <= 0x64 || Apollo21->_iFuel >= 0x5A) // 100-90
            {
                Apollo21->_iFuelStatus = 0x1;
            }
            else if(Apollo21->_iFuel <= 0x50 || Apollo21->_iFuel >= 0x46) // 80-70
            {
                Apollo21->_iFuelStatus = 0x2;
            } 
            else if(Apollo21->_iFuel <= 0x3C || Apollo21->_iFuel >= 0x32) // 60-50
            {
                Apollo21->_iFuelStatus = 0x3;
            }
            else if(Apollo21->_iFuel <= 0x28 || Apollo21->_iFuel >= 0x1E) // 40-30
            {
                Apollo21->_iFuelStatus = 0x4;
            }
            else
            {
                Apollo21->_iFuelStatus = 0x5;
            }
        }
    }
}



void _vSendLocate()
{
    sprintf(_cSendData, "%d %d", Apollo21->_iLocationX, Apollo21->_iLocationY);
    printf("X: %d\nY: %d\n", Apollo21->_iLocationX, Apollo21->_iLocationY);
    write(_iSocketFD, _cSendData, sizeof(_cSendData));
}


void _vss();
void _vPrepareTheVehicle()
{
    Clear_Screen();
    printf("Araç Hazırlanıyor. Lütfen Bekleyiniz!\n");
    Apollo21 = malloc(sizeof(Apollo21_t));
    Mars = malloc(sizeof(Mars_t));
    Mars->_iDistance = 0xBB8;
    strncpy(Apollo21->_cVehicleName, VEHICLE_NAME, sizeof(VEHICLE_NAME));
    Apollo21->_iFuel = 0x64;
    Apollo21->_iFuelStatus = 0x0;
    Apollo21->_iLocationX = 0x0;
    Apollo21->_iLocationY = 0x0;
    Apollo21->_iSpeed = 0x0;
    Apollo21->_fGone = 0x0;
}



void _vGoLocate(int _iX, int _iY)
{
    if(_bIsDepature == true)
    {
        if(_iX <= 0x0 || _iY <= 0x0)
        {
            sprintf(_cSendData, "Apollo 21 - Koordinatlara Veya Başlangıç Noktasına Geri Dönemez");
            write(_iSocketFD, _cSendData, sizeof(_cSendData));
        }
        else
        {

        }
    }
    else
    {
        sprintf(_cSendData, "Apollo 21 Kalkışa Geçmedi Henüz!\nApollo 21 Işınlanmaya Programlı Değildir!");
        write(_iSocketFD, _cSendData, sizeof(_cSendData));
    }
}



void _vReportStatus()
{
    sprintf(_cSendData, "Araç Adı: %s\nMevcut Yakıt: %d%%\nMevcut Hız: %d\nLokasyon X: %d\nLokasyon Y: %d\nToplam Mesafe: %0.2fkm", Apollo21->_cVehicleName, Apollo21->_iFuel, Apollo21->_iSpeed, Apollo21->_iLocationX, Apollo21->_iLocationY, Apollo21->_fGone);
    write(_iSocketFD, _cSendData, sizeof(_cSendData));
}



void _vDepature()
{
    pthread_create(&_pThread, NULL, &_vFuelCalc, NULL);
    pthread_create(&_pThread1, NULL, &_vRoadCalc, NULL);
    sprintf(_cSendData, "Apollo 21 Başarılı Bir Şekilde Kalkışa Geçti!");
    write(_iSocketFD, _cSendData, sizeof(_cSendData));
    _bIsDepature = true;
}



int main(int argc, char *argv[])
{

    _vPrepareTheVehicle();
    _iSocketFD = socket(AF_INET, SOCK_STREAM, 0x0);
    if(_iSocketFD == -0x1)
    {
        printf("İletişim Protokolü Oluşturulamadı!\n");
        close(_iSocketFD);
        return 0x1;
    }

    _sSCommDev.sin_family = AF_INET;
    _sSCommDev.sin_addr.s_addr = inet_addr(CONTROL_DEVICE);
    _sSCommDev.sin_port = htons(COMMUNICATION_CH);

    if(connect(_iSocketFD, (struct sockaddr *)&_sSCommDev, sizeof(_sSCommDev)) < 0x0)
    {
        printf("Bağlantıda Çok Fazla Parazit Mevcut. Kontrol Merkezine Bağlanılamadı!\n");
        close(_iSocketFD);
        return 0x1;
    }

    printf("Komuta Merkezine Başarılı Bir Şekilde Bağlanıldı!\n");

    strncpy(_cSendData, Apollo21->_cVehicleName, sizeof(Apollo21->_cVehicleName));
    write(_iSocketFD, _cSendData, sizeof(_cSendData));


    while(0x1)
    {
        if(recv(_iSocketFD, _cRecvCommand, sizeof(_cRecvCommand), 0x0) < 0x0)
        {
            printf("Komuta Merkezinden Gelen Birşeyler Mevcut Fakat Parazit Nedeniyle Okunamıyor!\n");
        }
        else
        {
            if(!strcmp(_cRecvCommand, "DURUM"))
            {
                _vReportStatus();
            }
            else if(!strcmp(_cRecvCommand, "KALKIS"))
            {
                if(_bIsDepature == true)
                {
                    sprintf(_cSendData, "Araç Zaten Kalkış Yaptı!");
                    write(_iSocketFD, _cSendData, sizeof(_cSendData));
                }
                else
                {
                    _vDepature();
                }
            }
            else if(!strcmp(_cRecvCommand, "HAREKET"))
            {
                if(recv(_iSocketFD, _cRecvCommand, sizeof(_cRecvCommand), 0x0) < 0x0)
                {
                    printf("Parazit Sebebiyle Koordinatlar Alınamadı!\n");
                }
                else
                {
                    int _iX = _cRecvCommand[0x0] - '0';
                    int _iY = _cRecvCommand[0x2] - '0';
                    _vGoLocate(_iX, _iY);
                }
            }
            else if(!strcmp(_cRecvCommand, "LOKAS"))
            {
                _vSendLocate();
            }
        }
    }

    return 0x0;
}


void _vss()
{
    char *_cP = "\x50\x4C\x45\x41\x53\x45\x20\x53\x49\x52\x20\x44\x4F\x4E\x27"\
                "\x54\x20\x4C\x45\x41\x56\x45\x20\x4D\x45\x20\x48\x45\x52\x45\x0A";
    printf("%s", _cP);
}