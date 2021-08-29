# smartlight
2021 서울 하드웨어 해커톤 프로젝트

## 팀명 및 팀원
* 김윤희 - 기획
* 김지상 - 개발

## 프로젝트 제목
* smartlight (스마트조명)

## 프로젝트 배경 혹은 목적
* C# Xamarin 및 타이젠 NUI 기반으로, 원격 조정이 가능한 스마트 조명 제작을 목표로 합니다.
* 하드웨어는 LED 하나를 두고, 디스플레이에 들어갈 소프트웨어 입니다.
![smartlight](https://user-images.githubusercontent.com/30719956/131217752-a24c508e-6b95-4d35-af11-a4a9c29845c6.jpeg)
![Image from iOS](https://user-images.githubusercontent.com/30719956/131238196-443411e0-1667-489c-894f-714c54f02b17.jpg)

## 파일 리스트 및 코드 기여자
* 아래는 파일은 직접 수정하고 기여한 부분 입니다
  * RESTClient.cs - REST API 서버 통신 모듈
  * RGBMixer.cs - RGB LED 컬러 설정 모듈
  * Scene1Page.xaml.cs - smartlight 메인 모듈

## 보드
* 이번 프로젝트에서는 RPi4 한대만 사용했습니다
* RPi4: GPIO 및 REST API 연동
  * https://tizenschool.org/tutorial/250/contents/1
  * https://www.csharpstudy.com/web/article/16-HttpWebRequest-%ED%99%9C%EC%9A%A9
  * RGB LED 140c05: https://www.aliexpress.com/i/4000140556861.html
![Image from iOS (1)](https://user-images.githubusercontent.com/30719956/131238367-bc84e4b1-e5b8-4c95-b6af-5443fda241cd.jpg)

## 구현사항
* Peripheral GPIO, PWM, REST API Client

## 시연영상
[YouTube Link](https://youtube.com/playlist?list=PLIRH65toTg8v9JBRHs-t288eE6ZGKI7L9)
