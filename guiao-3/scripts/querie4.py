commits = open("../entrada/commits-final.csv", "r")
users = open("../entrada/users-final.csv", "r")
expected_result = open("../expected/q4_exp_res.txt","w")

users_lines = users.readlines()
commits_lines = commits.readlines()

expected_result.write("{:.2f}".format((len(commits_lines)-1)/(len(users_lines)-1)))

commits.close()
users.close()
expected_result.close()