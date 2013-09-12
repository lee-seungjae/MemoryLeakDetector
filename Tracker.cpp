#include "Tracker.h"

// 속도가 대단히 중요하기 때문에 버킷 사이즈를 크게 잡는다.
int bucketSize = 1024 * 1024;

void Tracker::Allocated(void* ptr, size_t size)
{

}

void Tracker::Deallocated(void* ptr)
{
}
/*
Seungjae: 음 뭘 하고 싶었냐면21:04

Seungjae: 릭 디텍터를 만들 건데21:04

Seungjae: 종료시점에 해제안되고 남은걸 쭉 출력해주는거 많이들 쓰는데21:04

Seungjae: 이거 단점이 고아된 메모리만 찾아주는거지21:05

Seungjae: 고아는 아닌데 의미없게 메모리 사용량을 늘리는 경우는 못찾아준단 말이지21:05

Seungjae: 그리고 어디서 할당했는지를 알려주지 않기때문에21:06

Seungjae: 릭이 있단걸 알고나서 고치기까지 비용도 들어21:06

Seungjae: 그래서 어떻게 할거냐면21:06

Seungjae: 프로그램이 로딩 다 되고나서 그 시점부터 malloc하고 free를 후킹해서21:07

Seungjae: 그 시점이후에 할당된 메모리들을 추적한다21:07

Seungjae: 시간이 충분히 지나서 메모리 사용량이 쭉 오르고 나면21:08

Seungjae: 대체로 릭인 메모리가 추적대상 중 최상위에 있겠지?21:08

Seungjae: 근데 할당해제를 추적하면서 이걸21:09

Seungjae: Malloc시점의 콜스택의 요약본으로 카테고라이즈한다21:09

Seungjae: 수만종류의 콜스택에서 수백만번의 할당을 할건데21:10

Seungjae: 이중 해제되지않는 메모리르 많이 할당할 콜스택응 뽑을수 있도21:10

Seungjae: 충분히 측정해서 용의자를 좁히고나면21:10

Seungjae: 용의자가 또 나타나쓸때 브레이크포인트를 건다21:11

Seungjae: 끝21:11

voldemort: 음 엔씨때 코드에 그런게 있었는데. 아마 엘투의 유산잀. 21:41

Seungjae: 음..23:35

Seungjae: 할당 없는 해시맵을 만들어야하네;23:36

Seungjae: 메모리 많이 처먹어도 되니까 아주아주 빠른 해시맵이 필요23:36

Seungjae: 그리고 멀티맵임23:37

Seungjae: 와 이게 의외로 어렵구나!1;;23:40

Seungjae: 어쩌지?23:40

Seungjae: 우유마시면서 고민해보자23:41

Seungjae: 속도쓰린데23:41

Seungjae: 우유를 마시니 방법이 떠올랐다23:44

Seungjae: 1. 콜스택을 그대로 유지하지 않는다. 걍 해시해버림23:44

Seungjae: 2. 할당할 때 8바이트를 키워서 할당하고, 거기에 스택 해시랑 할당 사이즈를 같이 집어넣어둔다.23:46

Seungjae: 3. 할당할 때 해시맵[스택해시] += 할당 사이즈;23:46

Seungjae: 4. 해제할 때는 포인터만 날라오지만 거기서 스택 해시랑 할당 사이즈를 꺼낼 수 있으니까 해시맵[스택해시] -= 할당 사이즈;23:47

Seungjae: 음23:47

Seungjae: 이러면23:47

Seungjae: malloc-free 동작을 변경하는 거니까 중간에 인젝션을 못하네?23:48

Seungjae: 해시맵을 두 개 써야겠다23:48

ipkn_: 인젝션한 애를 저장해두고 23:48

Seungjae: ㅇㅇ23:48

Seungjae: 그거.23:48

ipkn_: 호출한 결과 받은다음에 조작할 수도 있지않을까요 23:48

Seungjae: ㅇㅇ23:49

Seungjae: 그렇게 하면됨23:49

Seungjae: 근데23:49

ipkn_: 네 23:49

Seungjae: 스택해시는 충돌이 나도 괜찮은데23:49

Seungjae: 정확도가 떨어질 뿐이지 동작에는 문제가 음심23:49

ipkn_: 그쵸 23:49

Seungjae: 인젝션중에 할당한 놈은 충돌 해소를 해야하는데23:49

Seungjae: 어쩌지23:49

Seungjae: 복잡하게 만들기싫은데23:49

ipkn_: 인젝션한 코드 안에서 malloc 해야하나요 23:50

ipkn_: 해쉬테이블이라서?..23:50

Seungjae: malloc 해도되는데 안하고 싶..23:51

ipkn_: 그쵸 23:51

ipkn_: 윈도만 쓸꺼면 윈도우즈 메모리 함수 쓸수도 있을꺼구요 23:51

Seungjae: 해시맵 충돌해소는 리스트쓰는게 제일간단한데23:51

Seungjae: 아무리 간단해도 짜기 개귀찮..23:52

ipkn_: allocator를 갈아치는 무식한 방법도 있고요 23:53

Seungjae: 음23:53

Seungjae: 그러게23:53

Seungjae: 음23:53

Seungjae: 걍 STL 써도 되는구나?;23:53

Seungjae: 훅으로 진입한 다음에는 걍 malloc 멀쩡하게 부를수있음23:53

Seungjae: 언패치된 상태라..23:53

ipkn_: 해결 23:54

Seungjae: judy library 같은거 써보까..23:57

Seungjae: 소스 크다 포기23:58

Seungjae: 걍 STL쓰고 느리면그때생각23:58

Seungjae: 은 내일23:58

Seungjae: 오늘은 졸려23:58

Seungjae: 굿잡23:58

ipkn_: 좋은밤되세요 23:58

Seungjae: 아니23:59

Seungjae: 굿잠23:59

ipkn_: gj23:59

*/