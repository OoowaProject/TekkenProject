# Tekken8
철권8을 모작하는 레포입니다.

## 시작방법

### Clone Repository

> [!NOTE]  
> 원하는 레포의 주소를 가져옵니다.

![image](./Assets/Clone_Repo.gif)

### Generate Cpp Code

> [!WARNING]  
> CPP 코드로 작성된 unreal 의 경우 CPP코드를 검증 작업이 필요합니다.

![image](./Assets/Generate.gif)

### Visual Studio 2022 실행

> [!NOTE]  
> *.Sln 파일을 실행시킨다

sln파일을 실행시키는 것으로 CPP로 작성된 프로젝트를 빌드한다.

| | |
| --- | --- |
|![image](./Assets/VstudioBuild.gif)| ![image](./Assets/VstudioStart.gif) |

### Unreal Test

실행된 Unreal Project가 정상적으로 실행되는지 검증한다.

![image](./Assets/UnrealEngineStart.gif)

## 작성 주의사항

> [!WARNING]  
> 모든 코드는 겹치지 않게 사용하여야 한다.

- 개발자의 경우 자신의 작업파일은 ```Context```폴더안에 닉네임을 작성하여 기록한다.
- 코드를 올리기전에 모든 파일이 ```dev```Branch와 충돌이 없는지 확인을 한다.

## Rule Of PR

- ```main```Branch의 경우 1명의 코드 리뷰자가 승인이 되어야 ```Merge```가 완료된다
