# 🕹️ PM Project: 3D PACMAN (2024)

2024년 **프로그래밍방법론** 수업에서 진행한 OpenGL 기반의 3D 팩맨 게임 프로젝트입니다.

## 📝 프로젝트 소개
고전 게임 '팩맨'을 3D 환경으로 재구성한 게임입니다. 
OpenGL을 활용하여 캐릭터와 맵을 렌더링하고, 유령(Ghost)들의 AI 로직과 점수 시스템을 구현했습니다.

## 🛠️ 개발 환경 및 기술 스택
- **Language:** C++
- **Graphics Library:** OpenGL (freeglut, FreeImage)
- **IDE:** Visual Studio 2022
- **Platform:** x64 / Windows

## 🎮 주요 기능
- **3D Rendering:** 팩맨과 유령 캐릭터의 3D 모델링 및 맵 렌더링
- **Game Logic:** 캐릭터 이동, 충돌 감지(Collision Detection)
- **Score System:** 먹이 획득 시 점수 증가 및 스코어보드 구현
- **Audio:** 게임 시작, 이동, 아이템 획득 시 사운드 효과 재생

## ⌨️ 조작 방법
- **방향키:** 팩맨 이동
- **H 키:** 홈 화면으로 이동 (스코어보드에서)
- **ESC:** 게임 종료

## 📂 폴더 구조
- `PMProject_PACMAN_2024/`: 핵심 소스 코드 (.cpp, .h)
- `include/`, `lib/`: OpenGL 및 외부 라이브러리 설정 파일
- `*.wav`, `*.png`: 게임에 사용된 사운드 및 이미지 리소스