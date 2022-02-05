import sys
from my_utils import date_lower_arr

top_n = int(sys.argv[1])
date = list(map(lambda x: int(x),sys.argv[2].split('-'))) # YYYY-MM-DD
filename = sys.argv[3]

repos = open("../entrada/repos-final.csv", "r")
expected_result = open("../expected/" + filename + ".txt","w")

repos_lines = list(map(lambda x: x.split(';'),repos.readlines()))
repos_lines.pop(0)

lang_dic = {} # formato {language: n_commits}
for line in repos_lines:
    created_at = list(map(lambda x: int(x),line[8].split(' ')[0].split('-')))
    language = line[6]
    if not(date_lower_arr(created_at,date)) and language != "None":
        if language in lang_dic:
            lang_dic[language] += 1
        else: 
            lang_dic[language] = 1

lang_list = []
for k in lang_dic:
    lang_list.append((k,lang_dic[k]))

lang_list = sorted(lang_list, key = lambda x: x[1], reverse = True)
while top_n > 0:
    if lang_list != []:
        (lang,_) = lang_list.pop(0)
        expected_result.write(lang + "\n")
        top_n -= 1
    else:
        top_n = 0

expected_result.close()
repos.close()