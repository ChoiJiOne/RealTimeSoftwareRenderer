# Software Renderer : Without OpenGL, Vulkan, DirectX, Metal
- 3D Software Renderer을 Graphics API(OpenGL, Vulkan, DirectX, Metal)이나 Game Engine(Unity3D, Unreal Engine) 없이 직접 구현해본다.
- 비실시간 버전과 실시간 버전 렌더링 엔진을 구분하여 구현한다.

## 개요
렌더링 기술의 발전과 CPU, GPU와 같은 하드웨어 성능의 발전으로 컴퓨터를 활용하여 현실 세계에 가까운 장면을 실시간으로 생성할 수 있게 되었다. 그것을 가능하게 해주는 Graphics API로는 다음과 같은 API들이 있다.
- Microsoft : DirectX
- Apple : Metal
- Linux or Cross platform : Vulkan / OpenGL

특히 DirextX12, Metal, Vulkan은 차세대 Graphics API로, 하드웨어 드라이버의 의존을 줄이고 프로그래머가 조금 더 하드웨어를 제어할 수 있도록 설계되었다. 이들 API와 최신 GPU를 활용한다면 실시간 Ray Tracing도 
가능하다. 이들 API와 각종 게임 엔진(Unreal, Unity3D)등을 지탱하는 컴퓨터 그래픽스 이론과 그 구현을 명확하게 이해하기 위해 Windows 10 환경에서 비실시간 버전은 C++만을 사용하고, 실시간 버전은 C++와 Win32만을 활용하여 직접 구현해본다.


## 사용된 외부 라이브러리
외부 라이브러리를 최대한으로 줄이는 방향으로 제작하지만, Windows 환경에서 API로 처리할 수 있는 텍스쳐는 얼마 존재하지 않고, 그 이외의 타입을 적용하기 위해서는 복잡한 과정을 거쳐야 한다. 따라서, 텍스처를 처리하기 위해서는 외부 라이브러리를 사용해야
하는 것은 불가피하다. 그런데, C/C++에서는 고맙게도 texture를 손쉽게 처리해주는 외부 라이브러리가 존재한다. 심지어 .lib나 .dll 형식이 아니라서 그냥 프로젝트에 소스 파일을 추가하기만 한다면 사용할 수 있다. 라이브러리 링크는 아래와 같다.
이 이외의 라이브러리는 일체 사용하지 않는 것을 목표로 한다.
- https://github.com/nothings/stb


## 구현 단계 (노트)
- 


## 참고 자료
- 책
  - Physically Based Rendering (번역 : 물리 기반 렌더링)
  - Mathematics for 3D Game Programming and Computer Graphics (번역 : 3D 게임 프로그래밍 & 컴퓨터 그래픽을 위한 수학)
  - OpenGL로 배우는 3차원 컴퓨터 그래픽스
  - Computer Graphics Principles And Practice In C (원서)
  - Real Time Rendering 3판 (원서)
  - 수학으로 시작하는 3D 게임 개발
  - Directx 12를 이용한 3d 게임 프로그래밍 입문
- 참고 링크
  - 기본적인 그래픽스 이론1 : https://www.scratchapixel.com/index.php?redirect
  - 기본적인 그래픽스 이론2 : https://gabrielgambetta.com/computer-graphics-from-scratch/introduction.html
  - Win32 기반의 기본적인 소프트웨어 렌더링 구현 : https://blog.naver.com/PostList.nhn?blogId=kch8246&from=postList&categoryNo=18
  - 렌더링 파이프라인에서 동차 좌표계를 사용하는 이유 : https://enghqii.tistory.com/59
  - 쿼터니언 정리 : https://enghqii.tistory.com/63
  - 사원수 회전 : https://ghebook.blogspot.com/2020/09/quaternion-and-rotation.html
  - OpenGL Projection Matrix : http://www.songho.ca/opengl/gl_projectionmatrix.html
  - 기본적인 소프트웨어 렌더링 : https://trenki2.github.io/blog/2017/06/06/developing-a-software-renderer-part1/
  - Quake2 Engine 분석 : https://fabiensanglard.net/quake2/quake2_software_renderer.php
  - Tiny Renderer Source 설명 : https://github.com/ssloy/tinyrenderer/wiki
  - 자바 스크립트 기반의 기본적인 소프트웨어 렌더링 : https://www.davrous.com/2013/06/13/tutorial-series-learning-how-to-write-a-3d-soft-engine-from-scratch-in-c-typescript-or-javascript/
  - 소프트웨어 렌러딩 참고 링크 모음: https://github.com/Angelo1211/SoftwareRenderer/wiki/Rendering-References
- 참고 소스
  - https://github.com/Angelo1211/SoftwareRenderer
  - https://github.com/ggambetta/computer-graphics-from-scratch
  - https://github.com/ssloy/tinyrenderer
  - https://github.com/idgmatrix/g-matrix3d-neo
  - https://github.com/mmp/pbrt-v3
  - https://github.com/zauonlok/renderer
