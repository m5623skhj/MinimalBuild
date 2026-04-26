# Event-Driven Player Architecture (C++ / Static Library 기반)

## 📌 개요

본 프로젝트는 **게임 서버 로직을 모듈 단위로 분리**하고,
**Actor 모델 + 내부 EventBus 구조**를 결합하여 다음을 목표로 합니다:

* 기능 간 **강한 결합 제거**
* **빌드 의존성 최소화**
* **멀티스레드 안정성 확보**
* **확장 가능한 이벤트 기반 설계**

---

## 🧱 아키텍처 구조

### 전체 구조

```
[ 외부 시스템 (Map / Network / etc) ]
                ↓
        (Message Queue)
                ↓
         Player Actor
                ↓
        [ 내부 EventBus ]
         ├─ Inventory
         ├─ Quest
         └─ 기타 시스템
```

---

## ⚙️ 핵심 개념

### 1. Actor 모델

* Player 단위로 Actor 구성
* 외부 입력은 모두 메시지 큐로 전달
* 단일 스레드에서 순차 처리

```cpp
void Player::Update()
{
    while (queue.pop(msg))
    {
        Handle(msg);
    }
}
```

---

### 2. 내부 EventBus

* Player 내부 시스템 간 통신 담당
* 동기 방식 (same thread)
* publish-subscribe 구조

```cpp
class EventBus
{
public:
    using Handler = std::function<void(const Context&)>;

    void Subscribe(ContextItemType type, Handler handler)
    {
        handlers[type].push_back(handler);
    }

    void Publish(const Context& ctx)
    {
        auto it = handlers.find(ctx.type);
        if (it == handlers.end())
            return;

        for (auto& handler : it->second)
        {
            handler(ctx);
        }
    }

private:
    std::unordered_map<ContextItemType, std::vector<Handler>> handlers;
};
```

---

### 3. Context 기반 데이터 전달

* 타입 의존 제거를 위한 공통 구조

```cpp
struct Context
{
    ContextItemType type;
    std::vector<uint8_t> data
};
```

---

### 4. 이벤트 설계 원칙

#### ✔️ 이벤트는 “사실(Fact)”만 표현

```text
ItemRemoved          (O)
NoticeToQuestItemRemoved (X)
```

#### ✔️ “요청 / 결과” 분리

```
ItemCheckRequest
ItemCheckResult
```

#### ✔️ 수신자는 필터링으로 처리

```cpp
if (!IsQuestTargetItem(data->itemId))
    return;
```

---

## 🧩 모듈 구조

각 기능은 정적 라이브러리(.lib)로 분리:

```
Core
 ├─ Actor
     ├─ EventBus

Modules (.lib)
 ├─ Inventory.lib
 ├─ Quest.lib
 └─ 기타 기능
```

---

## 🔗 초기화 흐름

```cpp
Player player;

player.Init();

---

void Player::Init()
{
    inventory.Init(bus);
    quest.Init(bus);
}
```

---

## 🧠 Event Handler 구조

### Init: 이벤트 연결만 담당

```cpp
void Quest::Init(EventBus& bus)
{
    bus.Subscribe(EVT_ITEM_REMOVED,
        [this, &bus](const Context& ctx)
        {
            OnItemRemoved(ctx, bus);
        });
}
```

---

### 실제 로직은 함수로 분리

```cpp
void Quest::OnItemRemoved(const Context& ctx, EventBus& bus)
{
    auto* data = reinterpret_cast<const ItemRemoved*>(ctx.data);

    if (!IsQuestTargetItem(data->itemId))
        return;

    CompleteQuest(data->playerId, 1);
}
```

---

## 🔄 예시 흐름: 퀘스트 클리어

```
QuestClearRequest
 → ItemCheckRequest

ItemCheckResult (Inventory)
 → ItemRemoveRequest

ItemRemoved (Inventory)
 → QuestCleared
 → ItemAddRequest

ItemAdded (Inventory)
 → 완료
```

---

## ⚠️ 설계 시 주의사항

### 1. EventBus lifetime

* EventBus는 반드시 Actor 내부에 존재해야 함
* 구독자보다 늦게 파괴되어야 안전

---

### 2. 외부에서 EventBus 직접 접근 금지

```cpp
// ❌ 금지
player->GetBus().Publish(...);

// ✔️ 권장
player->SendMessage(...);
```

---

### 3. 이벤트 루프 주의

* 이벤트 재진입으로 인한 루프 가능성 존재
* 설계 단계에서 “Request / Result” 구분으로 방지

---

### 4. Subscribe는 Init에서만

```cpp
// ✔️ 권장
void Init(EventBus& bus);
```

---

## 🚀 장점

* 🔓 기능 간 완전 분리 (Inventory ↔ Quest)
* 🧩 모듈 단위 확장 가능
* 🧵 멀티스레드 안정성 확보

---

## 📌 한 줄 요약

> **Actor(외부 메시지) + EventBus(내부 이벤트)를 결합한,**
> **저결합 구조**

---

## 📎 향후 개선 방향

* 이벤트 로깅 및 추적 시스템
* 이벤트 우선순위 처리
* 이벤트 채널 분리 (고급)
* 테스트용 Mock EventBus

---

## 🏁 결론

이 구조는 다음을 만족합니다:

* lib/헤더 의존성 없이 모듈 분리
* 빌드 비용 최소화
* 확장 가능한 이벤트 기반 설계

---

## ✅ 해결해야 할 문제 (TODO)

* [ ] Pub/Sub 구조에서 발생할 수 있는 **이벤트 무한 루프 문제 해결**
  * 이벤트 핸들러 내부에서 다시 Publish가 호출되며 순환 구조가 발생할 수 있음
  * Request / Result 이벤트 분리로 순환 방지를 유도하지만, 단순 구조에서는 괜찮겠으나, 기능 확장에 따라 문제가 될 가능성이 여전히 보임
  * 디버깅을 위한 이벤트 트레이싱 로그 추가
