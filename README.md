# Raspberry pi 3 Security Iot AP

    이 프로젝트는 Raspberry pi3를 사용해서 보통 집안에 있는 공유기를 직접 구현하는 프로젝트이다.
    실제로 와이파이를 구축해 사용할 수 있을 뿐만 아니라 관리페이지를 통해 웹과 어플리케이션, 관리 및 모니터링을
    할 수 있고 Double SSID, led warning Alert, OTP 시스템등을 통해 보안적인 요소까지 구현되어있다.
 
# Qucik Start

### 1. C Core Server

    $ cd src
  
    $ make

    $ sudo ./run
 
### 2. Ruby on Rails Webserver

    $ cd SomaIotSecurity/SomaIotWeb
  
    $ thin -R config.ru -a 'your raspberry pi IP Address' -p 3000 start or rails s -b 'your raspberry pi IP Address'

### 3. Android App Client

    1) Install Android-Studio : https://developer.android.com/studio/index.html?hl=ko
  
    2) File -> Import Existing Project -> PATH_CLONE_SomaIotSecurity/SomaIotApp
  
# ScreenShot

// todo 실제 화면 사진
// todo 아니면 동영상링크

# References 

    - http://w1.fi/hostapd/ - hostpd
    - http://www.thekelleys.org.uk/dnsmasq/doc.html - dnsmasq
    - https://github.com/json-c/json-c - json-c
    - http://wiringpi.com/ - wiringapi

# Copyright

// todo 멤버들 각자 프로필

    Youngje jo [ siosio34@nate.com ] site : https://github.com/siosio34  
    Gi-yong Kwon [ handgear05@gmail.com ] site : https://github.com/handgear  
    DONGWOO KIM [ kdw8681@naver.com ] site : https://github.com/westwoods  

# License

The MIT License (MIT)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

