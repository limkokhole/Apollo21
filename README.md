# Apollo21
Apollo 21 source code and documentation

## Build

_iY
If arch is x86 then
```sh
$ gcc control.c -z execstack -m32 -fno-stack-protector -o control -w
$ gcc vehicle.c -z execstack -m32 -w -fno-stack-protector -o vehicle -lpthread
``` 
else arch's x64 
```sh
$ gcc control.c -z execstack -fno-stack-protector -o control -w
$ gcc vehicle.c -z execstack -w -fno-stack-protector -o vehicle -lpthread
``` 


## Usage

This project build on socket *control.c* file is the server and *vehicle.c*'s client. You should run it first *control.c*, after *vehicle.c*

```sh
$ ./control
Apollo 21 Komuta Merkezi Başlıyor!
``` 

```sh
$ ./vehicle
Araç Hazırlanıyor. Lütfen Bekleyiniz!
Komuta Merkezine Başarılı Bir Şekilde Bağlanıldı!
``` 

## Commands
If you want to see the control commands please use *YARDIM* command.

#### Example

```
Apollo 21 ile İletişim Sağlandı!
Verebileceğiniz Komutları Görebilmek İçin YARDIM Anahtar Kelimesini Kullanınız!

YARDIM
YARDIM	Apollo 21 İçin Kullanılabilir Komutları Gösterir.
DURUM	Apollo 21'in Güncel Durumunu Gösterir.
KALKIS	Apollo 21'in Kalkış İşlemlerini Başlatır
HAREKET	Apollo 21'i Belirtilen Koordinatlara Yönlendirir
LOKAS	Apollo 21'in Güncel Koordinatlarını Bildirir.
KONTROL	Apollo 21'in Kontrolünü, Kontrol Merkezine Bırakır
ORNEK	Apollo 21'in Son Karşılaştığı 3 Farklı Yüzey Birimini Gösterir
``` 

 - **DURUM**
 This command, show Apollo 21 status (**Fuel,Speed,Location**)
 
 - **KALKIS**
 This command, air the Apollo 21.
 
 - **HAREKET**
 This command, move the Apollo 21 **X,Y** coordinate.
 
 - **LOKAS**
 This command, get the Apollo 21 **X,Y** coordinate.
 
 - **KONTROL**
 This command, permission to control the Apollo 21
 
 - **ORNEK**
 This command, collect the floor samples.


## Problems And Solutions

#### Hata! Bind Başarısız!

This problem is caused by socket.

```sh
$ ps aux | grep control
xt0xid     3270  0.0  0.0   4336   656 pts/5    T    20:43   0:00 ./control
``` 

```sh
$ kill -9 3270
[1]+  Killed     ./control
``` 