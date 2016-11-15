/*
        Project: Apollo 21
        Filename: control.c
        Author: Ömer Furkan
        Contact: omer.furkan.uyanik1@gmail.com
        Date: 12 Kasım 2016 - 15:47

        x86 BUILD
        gcc control.c -z execstack -m32 -fno-stack-protector -o control
        x64 BUILD
        gcc control.c -z execstack -fno-stack-protector -o control

    Marsa gönderilecek aracı komuta eden işlemlerin bulunduğu kaynak dosyası
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>


#define COMMUNICATION_CH        0x539
#define VEHICLE_NAME            "Apollo 21"

#define Clear_Screen()          printf("\033[H\033[J")



void _vHelp()
{
      printf("YARDIM\tApollo 21 İçin Kullanılabilir Komutları Gösterir.\n"\
         "DURUM\tApollo 21'in Güncel Durumunu Gösterir.\n"\
         "KALKIS\tApollo 21'in Kalkış İşlemlerini Başlatır\n"
         "HAREKET\tApollo 21'i Belirtilen Koordinatlara Yönlendirir\n"\
         "LOKAS\tApollo 21'in Güncel Koordinatlarını Bildirir.\n"\
         "KONTROL\tApollo 21'in Kontrolünü, Kontrol Merkezine Bırakır\n"\
         "ORNEK\tApollo 21'in Son Karşılaştığı 3 Farklı Yüzey Birimini Gösterir\n");
}



void _vRecvError()
{
    printf("Parazitten Dolayı Apollo 21'den Gelen Veriler Okunamadı!\n");
}


int main(int argc, char *argv[])
{
    int _iListenFD = 0x0, _iConnFD = 0x0, _iCheck = 0x0;
    struct sockaddr_in _sSCommDev;
    struct sockaddr_in _sSApollo21;
    char _cReceiveData[0x400];
    char _cSendData[0x400];
    char *_cCommandInput = malloc(sizeof(char*) * 0xFF);


    _iListenFD = socket(AF_INET, SOCK_STREAM, 0x0);
    _sSCommDev.sin_family = AF_INET;
    _sSCommDev.sin_addr.s_addr = htonl(INADDR_ANY);
    _sSCommDev.sin_port = htons(COMMUNICATION_CH);

    if(bind(_iListenFD, (struct sockaddr*)&_sSCommDev, sizeof(_sSCommDev)) < 0x0)
    {
        printf("Hata! Bind Başarısız!\n");
        close(_iListenFD);
        return 0x1;
    }
    if(listen(_iListenFD, 0xA) < 0x0)
    {
        printf("Hata! Listen Fonksiyonu Bir Hata İle Karşılaştı!\n");
        close(_iListenFD);
        return 0x1;
    }

    printf("Apollo 21 Komuta Merkezi Başlıyor!\n");


    while(0x1)
    {
        socklen_t _sApollo21Lenght = sizeof(_sSApollo21);
        _iConnFD = accept(_iListenFD, (struct sockaddr*)&_sSApollo21, &_sApollo21Lenght);
        _iCheck = recv(_iConnFD, _cReceiveData, sizeof(_cReceiveData), 0x0);
        if(_iCheck < 0x0)
        {
            printf("Hata! Çok Fazla Parazit Mevcut Bilgiler Tam Alınamadı!\n");
            close(_iListenFD);
            close(_iConnFD);
            return 0x1;
        }
        else
        {
            Clear_Screen();
            printf("Apollo 21 ile İletişim Sağlandı!\nVerebileceğiniz Komutları Görebilmek İçin YARDIM Anahtar Kelimesini Kullanınız!\n");
            while(0x1)
            {
                scanf("%50s", _cCommandInput);
                if(!strcmp(_cCommandInput, "YARDIM"))
                {
                    _vHelp();
                }
                else if(!strcmp(_cCommandInput, "DURUM"))
                {
                    sprintf(_cSendData, "DURUM");
                    write(_iConnFD, _cSendData, sizeof(_cSendData));
                    _iCheck = recv(_iConnFD, _cReceiveData, sizeof(_cReceiveData), 0x0);
                    if(_iCheck < 0x0)
                    {
                        _vRecvError();
                    }
                    else
                    {
                        printf("%s\n", _cReceiveData);
                    }
                }
                else if(!strcmp(_cCommandInput, "KALKIS"))
                {
                    sprintf(_cSendData, "KALKIS");
                    write(_iConnFD, _cSendData, sizeof(_cSendData));
                    _iCheck = recv(_iConnFD, _cReceiveData, sizeof(_cReceiveData), 0x0);
                    if(_iCheck < 0x0)
                    {
                        _vRecvError();
                    }
                    else
                    {
                        printf("%s\n", _cReceiveData);
                    }
                }
                else if(!strcmp(_cCommandInput, "HAREKET"))
                {
                    sprintf(_cSendData, "HAREKET");
                    write(_iConnFD, _cSendData, sizeof(_cSendData));
                    int _iXX, _iYY;
                    printf("X Koordinatını Giriniz: ");
                    scanf("%d", &_iXX);
                    printf("Y Koordinatını Giriniz: ");
                    scanf("%d", &_iYY);
                    sprintf(_cSendData, "%d", _iXX);
                    write(_iConnFD, _cSendData, sizeof(_cSendData));
                    sprintf(_cSendData, "%d", _iYY);
                    write(_iConnFD, _cSendData, sizeof(_cSendData));
                    _iCheck = recv(_iConnFD, _cReceiveData, sizeof(_cReceiveData), 0x0);
                    if(_iCheck < 0x0)
                    {
                        _vRecvError();
                    }
                    else
                    {
                        printf("%s\n", _cReceiveData);
                    }
                }
                else if(!strcmp(_cCommandInput, "LOKAS"))
                {
                    sprintf(_cSendData, "LOKAS");
                    write(_iConnFD, _cSendData, sizeof(_cSendData));
                    _iCheck = recv(_iConnFD, _cReceiveData, sizeof(_cReceiveData), 0x0);
                    if(_iCheck < 0x0)
                    {
                        _vRecvError();
                    }
                    else
                    {
                        int _iX = atoi(_cReceiveData);
                        int _iY;
                        _iCheck = recv(_iConnFD, _cReceiveData, sizeof(_cReceiveData), 0x0);
                        _iY = atoi(_cReceiveData);
                        printf("Apollo 21'in Güncel Koordinatları\nX: %d Y: %d\n", _iX, _iY);
                    }
                }
            }
        }
    }

    return 0x0;
}