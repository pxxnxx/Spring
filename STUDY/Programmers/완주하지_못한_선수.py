def solution(participant, completion):
    a = sorted(participant)
    b = sorted(completion)
    i = 0

    while a[i] == b[i]:
        i += 1
        if i >= len(a)-1:
            break
    answer = a[i]
    return answer
