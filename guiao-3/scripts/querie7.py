import sys
from my_utils import date_lower_arr

date = list(map(lambda x: int(x),sys.argv[1].split('-'))) # YYYY-MM-DD

repos = open("../entrada/repos-final.csv", "r")
commits = open("../entrada/commits-final.csv", "r")
expected_result = open("../expected/q7_exp_res.txt","w")

repos_lines = list(map(lambda x: x.split(';'),repos.readlines()))
repos_lines.pop(0)
commits_lines = list(map(lambda x: x.split(';'),commits.readlines()))
commits_lines.pop(0)

repos_dic = {} # formato {repo_id: (description,n_commits)}
for line in repos_lines:
    repos_dic[int(line[0])] = (line[5],0)

for line in commits_lines:
    commit_at = list(map(lambda x: int(x),line[3].split(' ')[0].split('-')))
    if not(date_lower_arr(commit_at,date)): # not(commit_at < date) <=> commit_at >= date
        repo_id = int(line[0])
        (description,number_of_commits) = repos_dic[repo_id]
        repos_dic[repo_id] = (description,number_of_commits+1)

repos_list = []
for k in repos_dic:
    (description,number_of_commits) = repos_dic[k]
    repos_list.append((k,description,number_of_commits))

repos_list = sorted(repos_list,key = lambda x: x[2])

while repos_list[0][2] == 0:
    (repo_id,description,_) = repos_list.pop(0)
    expected_result.write(str(repo_id) + ";" + description + "\n")


repos.close()
commits.close()
expected_result.close()