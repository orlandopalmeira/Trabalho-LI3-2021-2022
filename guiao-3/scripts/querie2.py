users = open("../entrada/users-final.csv", "r")
repos = open("../entrada/repos-final.csv", "r")
commits = open("../entrada/commits-final.csv", "r")
expected_result = open("../expected/q2_exp_res.txt","w")

commits_per_user = {} # formato: {user_id: num_commits}

# Criação do dicionario com os users
user_lines = list(map(lambda x: x.split(';'),users.readlines()))
user_lines.pop(0)
for line in user_lines:
    commits_per_user[int(line[0])] = 0

# contagem dos commits por user registando os valores no dicionario
commits_lines = list(map(lambda x: x.split(';'),commits.readlines()))
commits_lines.pop(0)
for line in commits_lines:
    commits_per_user[int(line[1])] += 1
# funciona ate aqui

commits_per_user_values = list(filter(lambda x: x != 0,commits_per_user.values())) # lista de numero de commits por user
repos_lines = repos.readlines() # linhas de cada ficheiro dos repos
collabs = len(commits_per_user_values) # numero de colaboradores

expected_result.write("{:.2f}".format(collabs/(len(repos_lines)-1)) + "\n")

users.close()
repos.close()
commits.close()
expected_result.close()