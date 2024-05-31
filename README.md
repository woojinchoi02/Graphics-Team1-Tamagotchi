# Graphics-Team1-Tamagotchi

### 사용법
1. SFML라이브러리를 다운받습니다.
2. 프로젝트 속성 C/C++(일반) 추가 포함 디렉터리에서 SFML폴더의 include 폴더를 선택합니다.
3. 프로젝트 속성 링커(일반) 추가 라이브러리 디렉터리에서 SFML폴더의 lib 폴더를 선택합니다.
4. 프로젝트 속성 링커(입력) 추가종속성에서 아래의 코드를 추가 해줍니다.
   
                        sfml-main-d.lib
                        sfml-graphics-d.lib
                        sfml-window-d.lib
                        sfml-system-d.lib
                        sfml-audio-d.lib
5. SDL2.dll파일을 프로젝트 X64 Debug파일에 넣어줍니다.
   
![캡처](https://github.com/woojinchoi02/Graphics-Team1-Tamagotchi/assets/162526228/0bf29758-0517-4627-8581-f850b55a4a38)
![111](https://github.com/woojinchoi02/Graphics-Team1-Tamagotchi/assets/162526228/67457b9a-aa32-43e4-8574-096a12cdf485)
![캡처123](https://github.com/woojinchoi02/Graphics-Team1-Tamagotchi/assets/162526228/cf748d00-0bcb-48a2-8651-1de8aad67064)

프로그램 시작 시 총 3마리의 포켓몬중 한마리가 랜덤으로 선택됩니다.


![기본로직](https://github.com/woojinchoi02/Graphics-Team1-Tamagotchi/assets/162526228/09acfaae-9378-4381-9caf-0483344523f4)
시간이 지나거나 놀아주기, 청소등을 하면 청결도, 배고픔, 행복도가 오르고 내려가며 일정 레벨 도달 시 진화 합니다.
![진화](https://github.com/woojinchoi02/Graphics-Team1-Tamagotchi/assets/162526228/59fa740d-d7dd-400d-b186-de0e87320352)





