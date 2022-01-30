users_final = open("../entrada/users-final.csv","r") # ficheiro dos users
expected_result = open("../expected/q1_exp_res.txt","w") # ficheiro para guardar o resultado
lines = users_final.readlines() # linhas csv de casa user
lines = list(map(lambda line: line.split(";"),lines)) # as linhas passam a ser separadas pelo ';'
length = len(lines) # número de linhas 
user = 0   # contador de users
bot = 0    # contador de bots
org = 0    # contador de organizacoes

for i in range(length): #contagem
    if lines[i][2] == "User":
        user += 1
    elif lines[i][2] == "Organization":
        org += 1
    elif lines[i][2] == "Bot":
        bot += 1
    else:
        None

expected_result.write("Bot: " + str(bot) + "\n" + "Organization: " + str(org) + "\n" + "User: " + str(user) + "\n")
users_final.close()
expected_result.close()