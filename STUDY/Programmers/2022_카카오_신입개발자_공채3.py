import math
def solution(fees, records):
    answer = []
    rcd = []
    for x in records:
        rcd.append(x.split())
    arr = dict()
    fee = dict()
    time = [0] * len(rcd)
    test = [0] * len(rcd)
    for i in range(len(rcd)):
        fee[rcd[i][1]] = 0
        add = 0
        add += int(rcd[i][0][0]) * 600
        add += int(rcd[i][0][1]) * 60
        add += int(rcd[i][0][3]) * 10
        add += int(rcd[i][0][4])
        time[i] = add
    for i in range(len(rcd)):
        if rcd[i][2] == "IN":
            arr[rcd[i][1]] = time[i]
        else:
            fee[rcd[i][1]] += time[i] - arr[rcd[i][1]]
            del arr[rcd[i][1]]
    if len(arr) != 0:
        for x in arr:
            fee[x] += 24*60-1 - arr[x]
    fee = sorted(fee.items())
    for x in fee:
        temp = x[1] - fees[0]
        if temp < 0:
            temp = 0
        answer.append(fees[1]+math.ceil(temp/fees[2])*fees[3])
    return answer

solution([180, 5000, 10, 600],["05:34 5961 IN", "06:00 0000 IN", "06:34 0000 OUT", "07:59 5961 OUT", "07:59 0148 IN", "18:59 0000 IN", "19:09 0148 OUT", "22:59 5961 IN", "23:00 5961 OUT"])
solution([120, 0, 60, 591],["16:00 3961 IN","16:00 0202 IN","18:00 3961 OUT","18:00 0202 OUT","23:58 3961 IN"])
solution([1, 461, 1, 10],["00:00 1234 IN"])
