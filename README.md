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
