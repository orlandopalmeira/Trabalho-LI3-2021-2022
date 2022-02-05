users = open("../entrada/users-final.csv", "r")
commits = open("../entrada/commits-final.csv", "r")
repos = open("../entrada/repos-final.csv", "r")

users_lines = list(map(lambda x: x.split(';'),users.readlines()))
users_lines.pop(0)
commits_lines = list(map(lambda x: x.split(';'),commits.readlines()))
commits_lines.pop(0)
repos_lines = list(map(lambda x: x.split(';'),repos.readlines()))
repos_lines.pop(0)

total = 0

def strToArray(arraystr):
    return [] if arraystr == '[]' else list(map(lambda x: int(x),arraystr[1:len(arraystr)-1].split(',')))


#Commirs: repo_id;author_id;committer_id;commit_at;message
#Repos: id;owner_id;full_name;license;has_wiki;description;language;default_branch;created_at;updated_at;forks_count;open_issues;stargazers_count;size
#Users: id;login;type;created_at;followers;follower_list;following;following_list;public_gists;public_repos

for line in users_lines:
    total += 42 + len(line[1]) + len(strToArray(line[5]))*4 + len(strToArray(line[7]))*4

for line in commits_lines:
    total += 33

for line in repos_lines:
    total += 29 + len(line[5]) + len(line[6])

print("Memory: " + str(total) + " bytes")
