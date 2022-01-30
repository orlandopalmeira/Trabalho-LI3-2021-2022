import sys

top_n = int(sys.argv[1])

def strToArray(arraystr):
    return [] if arraystr == '[]' else list(map(lambda x: int(x),arraystr[1:len(arraystr)-1].split(',')))

users = open("../entrada/users-final.csv", "r")
repos = open("../entrada/repos-final.csv", "r")
commits = open("../entrada/commits-final.csv", "r")
expected_result = open("../expected/q9_exp_res.txt","w")

users_lines = list(map(lambda x: x.split(';'),users.readlines()))
users_lines.pop(0)
repos_lines = list(map(lambda x: x.split(';'),repos.readlines()))
repos_lines.pop(0)
commits_lines = list(map(lambda x: x.split(';'),commits.readlines()))
commits_lines.pop(0)

user_dic = {} # formato {user_id: (login,number_of_commits,followers,following)}
for line in users_lines:
    user_dic[int(line[0])] = (line[1],0,strToArray(line[5]),strToArray(line[7]))

repo_dic = {} # formato {repo_id: owner_id}
for line in repos_lines:
    repo_dic[int(line[0])] = int(line[1])

for line in commits_lines:
    author_id = int(line[1]) 
    repo_id = int(line[0])
    owner_id = repo_dic[repo_id]
    (login,number_of_commits,followers,following) = user_dic[author_id]
    if (owner_id in followers) and (owner_id in following):
        user_dic[author_id] = (login,number_of_commits+1,followers,following)

users_list = []
for k in user_dic:
    (login,number_of_commits,_,_) = user_dic[k]
    users_list.append((k,login,number_of_commits))

users_list = sorted(users_list, key = lambda x: x[2], reverse = True)

while top_n > 0:
    elem = users_list.pop(0)
    expected_result.write(str(elem[0]) + ";" + elem[1] + "\n")
    top_n -= 1

users.close()
repos.close()
commits.close()
expected_result.close()