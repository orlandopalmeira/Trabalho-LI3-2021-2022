import sys

users = open("../entrada/users-final.csv", "r")
repos = open("../entrada/repos-final.csv", "r")
commits = open("../entrada/commits-final.csv", "r")
expected_result = open("../expected/q6_exp_res.txt","w")

top_n = int(sys.argv[1])
language = sys.argv[2]

users_lines = list(map(lambda x: x.split(';'),users.readlines()))
users_lines.pop(0)
commits_lines = list(map(lambda x: x.split(';'),commits.readlines()))
commits_lines.pop(0)
repos_lines = list(map(lambda x: x.split(';'),repos.readlines()))
repos_lines.pop(0)

users_dic = {} # formato {user_id: (login,number_of_commits)}
for line in users_lines:
    users_dic[int(line[0])] = (line[1],0)

repos_dic = {} # formato {repo_id: language}
for line in repos_lines:
    repos_dic[int(line[0])] = line[6]

for line in commits_lines:
    author_id = int(line[1])
    repo_id = int(line[0])
    if repos_dic[repo_id] == language:
        (login,number_of_commits) = users_dic[author_id]
        users_dic[author_id] = (login,number_of_commits+1)

user_list = []
for k in users_dic:
    (login,number_of_commits) = users_dic[k]
    user_list.append((k,login,number_of_commits))

user_list = sorted(user_list, key = lambda x: x[2], reverse = True)
while top_n > 0:
    elem = user_list.pop(0)
    expected_result.write(str(elem[0]) + ";" + elem[1] + ";" + str(elem[2]) + "\n")
    top_n -= 1

expected_result.close()
repos.close()
commits.close()
users.close()