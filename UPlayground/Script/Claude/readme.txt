UPlayground 프로젝트 기능 분석 및 정리
UPlayground는 언리얼 엔진 5.4 기반의 모듈식 RPG 프로젝트로, C++ 20 표준을 사용하여 개발되고 있습니다. 현재까지 구현된 주요 시스템과 기능을 분석해 정리해보겠습니다.
1. 핵심 아키텍처
매니저 시스템

UPGManagerSubsystem: 게임 인스턴스 서브시스템으로 모든 매니저 관리 및 조정
UPGBaseManager: 모든 매니저의 기본 클래스
IPGIManager: 매니저 인터페이스
중앙 집중식 게임 상태 관리 (로딩, 메인 메뉴, 일시정지, 실행 등)
매니저 등록 및 해제 메커니즘

게임 피쳐 시스템

UPGGameFeatureManagerAction: 게임 피쳐 플러그인 액션 기본 클래스
UPGRPGManagersFeature: RPG 매니저에 특화된 게임 피쳐 구현
모듈식 게임 기능 구현을 위한 플러그인 아키텍처

2. 인벤토리 및 아이템 시스템
인벤토리 관리

UPGInventoryManager: 전역 아이템 관리 담당
UPGInventoryComponent: 캐릭터에 부착되는 인벤토리 컴포넌트
아이템 추가/제거/사용 기능
인벤토리 용량 관리
아이템 정렬 기능 (ID, 타입, 희귀도 기준)
자동 저장 메커니즘

아이템 시스템

UPGItem: 아이템 기본 클래스
UPGItemManager: 아이템 생성 및 관리
다양한 아이템 타입 지원 (기타, 소비품, 장비, 퀘스트)
아이템 희귀도 분류 (일반, 고급, 희귀, 에픽, 전설)
스택 가능한 아이템 시스템

3. 퀘스트 시스템
퀘스트 관리

UPGQuestManager: 퀘스트 진행 및 상태 관리
다양한 퀘스트 타입 지원 (메인, 사이드, 일일, 월드, 숨겨진 퀘스트)
퀘스트 상태 관리 (미시작, 진행 중, 완료, 실패, 포기)

퀘스트 목표 및 보상

다양한 퀘스트 목표 (적 처치, 아이템 수집, 상호작용, 에스코트, 장소 도달)
퀘스트 보상 시스템 (아이템, 통화, 경험치, 평판, 스킬)
시간 제한 퀘스트 지원
이벤트 기반 퀘스트 진행 추적

4. 캐릭터 시스템
기본 캐릭터 프레임워크

APGBaseCharacter: 모든 캐릭터의 기본 클래스
기본 속성 관리 (체력, 마나, 레벨 등)
상호작용 시스템
데미지 처리 및 사망 메커니즘

NPC 시스템

APGNPC: NPC 캐릭터 클래스
다양한 NPC 유형 (중립, 우호적, 적대적, 상인, 퀘스트 제공자)
대화 시스템 프레임워크
상점 및 퀘스트 제공 기능

플레이어 캐릭터

APGPlayerCharacter: 플레이어 캐릭터 클래스
로컬/리모트 플레이어 구분
RPG 능력치 시스템 (힘, 민첩, 지능, 체력)
경험치 및 레벨업 시스템
네트워크 복제 지원

플레이어 컨트롤러

APGPlayerController: 플레이어 입력 및 UI 관리
UI 모드 전환 (게임 전용, 게임+UI, UI 전용)
메뉴 시스템 (인벤토리, 퀘스트 로그, 지도, 일시정지)

5. 카메라 시스템
다중 시점 카메라

UPGCameraSystem: 카메라 모드 관리 컴포넌트
3가지 카메라 모드 지원 (1인칭, 3인칭, 자유시점)
모드 간 전환 기능

카메라 기능

3인칭 모드에서 줌 인/아웃 기능
자유시점 모드에서 독립적인 카메라 이동 및 회전
1인칭 모드에서 몰입형 시점

입력 처리

EnhancedInput 시스템 활용
카메라 전용 입력 액션 및 매핑 컨텍스트
다양한 입력 모드 지원 (키보드, 마우스, 컨트롤러)

6. 기술적 특징

C++ 20 표준 사용
언리얼 엔진 5.4 기반
게임 피쳐 플러그인 시스템 활용
모듈식 아키텍처로 확장성 제공
델리게이트를 통한 이벤트 기반 통신
EnhancedInput 플러그인 사용
CommonUI 플러그인 지원
네트워크 복제 지원 (멀티플레이어 준비)

7. 확장 가능성
현재 구조는 모듈화되어 있어 다음과 같은 RPG 요소를 쉽게 추가할 수 있도록 설계되어 있습니다:

전투 시스템
스킬 및 능력 시스템
더 발전된 대화 시스템
파티 관리 시스템
퀘스트 에디터 및 스크립팅 도구
아이템 제작 시스템
진행 저장 및 로드 시스템

UPlayground는 확장 가능한 RPG 프레임워크로서 모듈식 설계를 통해 새로운 기능과 콘텐츠를 쉽게 추가할 수 있는 강력한 기반을 제공합니다. 현재까지의 구현은 RPG 게임의 핵심 시스템을 중심으로 이루어져 있으며, 이를 바탕으로 다양한 RPG 장르의 게임을 개발할 수 있을 것입니다.