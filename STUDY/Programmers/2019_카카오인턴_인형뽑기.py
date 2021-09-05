def solution(board, moves):
    answer = 0
    arr = [0]
    c = 0
    for i in moves:
        i -= 1
        j = 0
        while board[j][i] == 0:
            if j == len(board)-1:
                c = 1    
                break
            j += 1
        if c == 1:
            c = 0
        elif arr[-1] == board[j][i]:
            del(arr[-1])
            answer += 2
        else:
            arr.append(board[j][i])
        board[j][i] = 0
    return answer
