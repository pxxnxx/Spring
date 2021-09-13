from collections import deque
def solution(cacheSize, cities):
    answer = 0
    queue = deque()
    cities = deque(cities)
    for i in range(len(cities)):
        cities[i] = cities[i].upper()
    if cacheSize == 0:
        return len(cities) * 5
    while len(queue) < cacheSize and cities:
        br = True
        for i in range(len(queue)):
            if queue[i] == cities[0]:
                del queue[i]
                answer += 1
                queue.append(cities.popleft())
                br = False
                break
        if br:
            queue.append(cities.popleft())
            answer += 5

    while cities:
        br = True
        for i in range(cacheSize):
            if queue[i] == cities[0]:
                answer += 1
                del queue[i]
                queue.append(cities.popleft())
                br = False
                break
        if br:
            queue.popleft()
            queue.append(cities.popleft())
            answer += 5
    print(answer)
    return answer
