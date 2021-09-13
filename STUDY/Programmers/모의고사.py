def solution(answers):
    answer = []
    a = [1,2,3,4,5]
    b = [2,1,2,3,2,4,2,5]
    c = [3,3,1,1,2,2,4,4,5,5]
    arr = [0, 0, 0]
    k = 0
    for i in range(len(answers)):
        if answers[i] == a[i%5]:
            arr[0] += 1
        if answers[i] == b[i%8]:
            arr[1] += 1
        if answers[i] == c[i%10]:
            arr[2] += 1
    for i in range(3):
        if k > arr[arr.index(max(arr))]:
            break
        answer.append(arr.index(max(arr))+1)
        k = arr[arr.index(max(arr))]
        arr[arr.index(max(arr))] = -1
    return answer
