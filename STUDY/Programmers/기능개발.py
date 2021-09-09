def solution(progresses, speeds):
    answer = []
    s = 0
    d = 0
    while s <= len(speeds)-1:
        for i in range(s,len(speeds)):
            progresses[i] += speeds[i]
        while progresses[s] >= 100:
            s += 1
            d += 1
            if s > len(speeds)-1:
                break
        if d != 0:
            answer.append(d)
            d = 0
    return answer
