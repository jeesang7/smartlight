## [Smartthings 개발 환경 준비 (Device key)](https://tizenschool.org/tutorial/254/contents/4)
### Device key 생성
- smartthings developer workspace에 생성한 key를 등록하여 device 접속 시 인증에 사용
- Keygen tool (https://github.com/SmartThingsCommunity/st-device-sdk-c/tree/master/tools/keygen)
- 다운로드 링크 :  https://raw.githubusercontent.com/SmartThingsCommunity/st-device-sdk-c/master/tools/keygen/stdk-keygen.py
- pynacl 패키지 설치
```bash
pip3 install pynacl --user
```
- 사용법
```bash
python3 stdk-keygen.py --mnid TEST --firmware V20200804
```
- 실행 예
```
$ python3 stdk-keygen.py --mnid TEST --firmware V20200804
Use following serial number and public key
for the identity of your device in Developer Workspace.

Serial Number:
STDKTEST****7KXw

Public Key:
Fqj6************************************hp0=

$ ls output_STDKTEST****7KXw -al
total 20
drwxrwxr-x 2 a a 4096  8월  6 13:57 .
drwxrwxr-x 3 a a 4096  8월  6 13:57 ..
-rw-rw-r-- 1 a a  222  8월  6 13:57 device_info.json
-rw-rw-r-- 1 a a   44  8월  6 13:57 device.pubkey.b64
-rw-rw-r-- 1 a a   44  8월  6 13:57 device.seckey.b64

$ cat output_STDKTEST****7KXw/device_info.json
{
	"deviceInfo": {
		"firmwareVersion": "V20200804",
		"privateKey": "H/Jy************************************1p4=",
		"publicKey": "Fqj6************************************hp0=",
		"serialNumber": "STDKTEST****7KXw"
	}
}
```
