from collections import deque
import re
import math

def solution(n,k):
    answer = 0
    t = deque()
    comp = []
    temp = []
    while n != 0:
        t.appendleft(str(n%k))
        n = n//k
    string = "".join(t)
    temp.append(re.compile('[1-9]+').findall(string))
    for x in temp[0]:
        comp.append(int("".join(x)))
    for x in comp:
        br = True
        if x == 2:
            answer += 1
        elif x != 1:
            for i in range(2,math.ceil(x**0.5)):
                if x % i == 0:
                    br = False
                    break
            if br:
                answer += 1
    print(answer)
    return answer
solution(437674,3)
