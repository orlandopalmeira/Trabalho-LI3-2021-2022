users = open("../entrada/users-final.csv", "r")
commits = open("../entrada/commits-final.csv", "r")
expected_result = open("../expected/q3_exp_res.txt","w")

bots = [] # lista de bots 
users_lines = list(map(lambda x: x.split(";"),users.readlines()))
users_lines.pop(0)
for line in users_lines: # cria a lista com os id's dos bots
    if line[2] == "Bot" :
        bots.append(int(line[0]))

reposWithBots = {} # formato {repo_id: has_commit_from_bot(boolean)}
commits_lines = list(map(lambda x: x.split(";"),commits.readlines()))
commits_lines.pop(0)
for line in commits_lines:
    if int(line[1]) in bots: #verifica se é um bot
        reposWithBots[int(line[0])] = True

result_list = list(filter(lambda x: x == True,reposWithBots.values()))
expected_result.write(str(len(result_list)) + "\n")

users.close()
commits.close()
expected_result.close()