#### PART 1 ####

# getfinalGrade: calculate the final grade of each student by instructions of *pdf.
#   id: its a string present student id.
#   grade: string that presents exam grade.
# Return student's final garde .
def getfinalGrade(id: str,grade: str)->int:
    scale = int(id[-2])*10 + int(id[-1])
    return int(((scale+int(grade))/2))


# checkDetails: check student's exam details from the input file for specific conditions by instructions of *pdf.
#   s1: string contains student details.
#   Return: 
#   True - if the details are llegal.
#   False -  if the details are illegal.
def checkDetails(s1: str)->bool:
    split_line = s1.split(',')
    for i, e in enumerate(split_line):
        if((i==0 and e[0]=='0') or (i==0 and (not (numberOfDigits(e)==8)))):
            return bool(0)
        if(i==1 and (not bool(e.isalpha()))):
            return bool(0)
        if(i==2 and int(e)<1):
            return bool(0)
        if(i==3 and (int(e)>100 or int(e)<=50)):
            return bool(0)
    return bool(1)



# numberOfDigits - counts how many digits in the string
# s1 - the string to check
# Return - number of digit in the string
def numberOfDigits(s1: str)->int:
    count=0
    for letter in s1:
        count+=1
    return count



# final_grade: Calculates the final grade for each student, and writes the output (while eliminating illegal
# rows from the input file) into the file in `output_path`. Returns the average of the grades.
#   input_path: Path to the file that contains the input.
#   output_path: Path to the file that will contain the output.
def final_grade(input_path: str, output_path: str) -> int:
    read_file = open(input_path,'r')
    write_file = open(output_path,'w')
    grade_dict = {}
    for line in read_file:
        clean_line = line.replace(" ","").replace("\n","")
        if checkDetails(clean_line): #Fill the dictanery with llegal details
            split_line = clean_line.split(',')
            separator = ", "
            grade_dict[split_line[0]]=separator+str(int(split_line[3]))+separator+str(getfinalGrade(split_line[0],split_line[3]))
    for key in sorted(grade_dict): #write to output file 
        write_file.write(key+grade_dict[key]+'\n')
    sum=0
    count=0
    for v in grade_dict.values(): #calculates grade avarege 
        sum+=int(v[-2])*10 + int(v[-1])
        count+=1
    read_file.close()
    write_file.close()
    if(count==0):
        return 0
    return int(sum/count)






#### PART 2 ####
# check_strings: Checks if `s1` can be constructed from `s2`'s characters.
#   s1: The string that we want to check if it can be constructed
#   s2: The string that we want to construct s1 from
def check_strings(s1: str, s2: str) -> bool:
    s1_lower = s1.lower()  
    s2_lower = s2.lower()
    new_dict = {}
    for c in s2_lower:
        if(c not in new_dict):new_dict[c]=1
        else:new_dict[c]+=1       
    for c in s1_lower:
        if((c in new_dict) and (not new_dict[c]==0)):
            new_dict[c] -=1
            continue
        return False
    return True