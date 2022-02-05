import sys
from my_utils import date_between_arr
top_n = int(sys.argv[1])
date1 = list(map(lambda x: int(x),sys.argv[2].split('-')))
date2 = list(map(lambda x: int(x),sys.argv[3].split('-')))
filename = sys.argv[4]

commits = open("../entrada/commits-final.csv", "r")
users = open("../entrada/users-final.csv", "r")
expected_result = open("../expected/" + filename + ".txt","w")

commits_lines = list(map(lambda x: x.split(';'),commits.readlines()))
commits_lines.pop(0) # ignorar a 1.ª Linha
users_lines = list(map(lambda x: x.split(';'),users.readlines()))
users_lines.pop(0) # ignorar a 1.ª Linha

user_dic = {} # formato {user_id: (login,number_of_commits)}
for line in users_lines:
    user_dic[int(line[0])] = (line[1],0)
    
for line in commits_lines:
    commit_at = list(map(lambda x: int(x),line[3].split(' ')[0].split('-')))
    if date_between_arr(date1,commit_at,date2):
        author_id = int(line[1])
        (login,number_of_commits) = user_dic[author_id]
        user_dic[author_id] = (login,number_of_commits+1)

user_list = []
for k in user_dic:
    user_list.append((k,user_dic[k][0],user_dic[k][1]))

user_list = sorted(user_list, key = lambda x: x[2], reverse = True)
while top_n > 0:
    elem = user_list.pop(0)
    expected_result.write(str(elem[0]) + ";" + elem[1] + ";" + str(elem[2]) + "\n")
    top_n -= 1

expected_result.close()
users.close()
commits.close()